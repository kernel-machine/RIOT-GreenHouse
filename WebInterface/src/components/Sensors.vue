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
    <div>
      <p><b>Last update: </b>{{ this.formatTimestamp(this.$data.last_update.timestamp) }}</p>
      <table style="margin: auto">
        <tr>
          <td>
            <p><b>Water level: </b>{{ this.$data.last_update.water_level }}%</p>
          </td>
          <td>
            <p><b>Window: </b>{{ this.$data.last_update.servo === 180 ? 'open' : 'close' }}</p>
          </td>
          <td>
            <p><b>Pump state: </b>{{ this.$data.last_update.pump === 1 ? 'on' : 'off' }}</p>
          </td>
        </tr>

        <tr>
          <td>
            <p><b>Temperature: </b>{{ this.$data.last_update.temperature }} °C</p>
          </td>
          <td>
            <p><b>Humidity: </b>{{ this.$data.last_update.humidity }} %</p>
          </td>
          <td>
            <p><b>Soil moisture: </b>{{ this.$data.last_update.soil }} %</p>
          </td>
        </tr>
      </table>
    </div>

    <ButtonPanel></ButtonPanel>
    <div v-if="this.$data.received_data.length>0" class="flex_container">

      <my-graph class="graph" v-bind:values="this.$data.received_data.map(x=>x.temperature)"
                v-bind:ts="this.$data.received_data.map(x=>x.timestamp)" name="Temperature" :color="this.TEMP_COLOR">
      </my-graph>

      <my-graph class="graph" v-bind:values="this.$data.received_data.map(x=>x.humidity)"
                v-bind:ts="this.$data.received_data.map(x=>x.timestamp)" name="Humidity" :color="this.HUMIDITY_COLOR">
      </my-graph>

      <my-graph class="graph" v-bind:values="this.$data.received_data.map(x=>x.soil)"
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
    this.loadData();
    setInterval(this.loadData, 60000);
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
.graph {
  flex: 30%;
}

.flex_container {
  display: flex;
  flex-direction: row;
}

td {
  padding: 10px;
}

@media screen and (max-width: 1000px) {
  .graph {
    flex: 100%;
  }

  .flex_container {
    display: flex;
    flex-direction: column;
  }
}
</style>