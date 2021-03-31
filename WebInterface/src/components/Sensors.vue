<template>
  <div>
    <loading :active.sync="isLoading"
             :can-cancel="false"
             :is-full-page="true"></loading>
    <MinMaxAvg title="Temperature" unit="°C" :color="this.TEMP_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.temperature)"></MinMaxAvg>
    <MinMaxAvg title="Humidity" unit="%" :color="this.HUMIDITY_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.humidity)"></MinMaxAvg>
    <MinMaxAvg title="Soil moisture" unit="%" :color="this.SOIL_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.soil)"></MinMaxAvg>
    <br>
    <p><b>Last update: </b>{{ this.formatTimestamp(this.$data.last_update.timestamp) }}</p>
    <p><b>Water level: </b>{{ this.$data.last_update.water_level }}%</p>
    <p><b>Window: </b>{{ this.$data.last_update.servo == 180 ? 'open' : 'close' }}</p>
    <p><b>Pump state: </b>{{ this.$data.last_update.pump == 1 ? 'on' : 'off' }}</p>

    <ButtonPanel></ButtonPanel>
    <div v-if="this.$data.received_data.length>0" style="display: flex; flex-direction: row;">

      <my-graph style="flex: 30%" v-bind:values="this.$data.received_data.map(x=>x.temperature)"
                v-bind:ts="this.$data.received_data.map(x=>x.timestamp)" name="Temperature" :color="this.TEMP_COLOR">
      </my-graph>

      <my-graph style="flex: 30%" v-bind:values="this.$data.received_data.map(x=>x.humidity)"
                v-bind:ts="this.$data.received_data.map(x=>x.timestamp)" name="Humidity" :color="this.HUMIDITY_COLOR">
      </my-graph>

      <my-graph style="flex: 30%" v-bind:values="this.$data.received_data.map(x=>x.soil)"
                v-bind:ts="this.$data.received_data.map(x=>x.timestamp)" name="Soil moisture" :color="this.SOIL_COLOR">
      </my-graph>

    </div>
  </div>
</template>

<script>
// Import component
import Loading from 'vue-loading-overlay';
// Import stylesheet
import 'vue-loading-overlay/dist/vue-loading.css';
import MyGraph from "@/components/MyGraph";

const axios = require('axios').default;

import MinMaxAvg from "@/components/MinMaxAvg";
import moment from "moment"
import ButtonPanel from "@/components/ButtonPanel";

export default {
  name: "Sensors",
  components: {
    ButtonPanel,
    MyGraph,
    MinMaxAvg,
    Loading
  },
  data: function () {
    return {
      fetch_data_url: "https://p4az3xm3gf.execute-api.us-east-1.amazonaws.com/default/greenHouseDataJS",
      received_data: [],
      last_update: {},
      TEMP_COLOR: "#ef476f",
      HUMIDITY_COLOR: "#ffd166",
      SOIL_COLOR: "#06d6a0",
      isLoading: true,
    }
  },
  mounted() {
    this.loadData()
  },
  methods: {
    formatTimestamp(ts) {
      return moment(ts).format("LLL")
    },
    loadData() {
      axios.post(this.$data.fetch_data_url)
          .then(d => {
            this.$data.received_data = d.data.Items

            const ordered_received_data = this.$data.received_data.sort((x, y) => {
              if (x.timestamp < y.timestamp) return -1;
              else if (x.timestamp > y.timestamp) return 1;
              else return 0
            })
            this.$data.last_update = ordered_received_data[ordered_received_data.length - 1];

          })
          .finally(() => {
            this.isLoading = false;
          })
    }
  }
}
</script>

<style scoped>

</style>