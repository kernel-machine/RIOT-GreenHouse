<template>
  <div>
    <loading :active.sync="isLoading"
             :can-cancel="false"
             :is-full-page="true"></loading>
    <a>Select a node </a>
    <select v-if="this.devices.length>0" v-model="selectedDevice">
      <option v-for="item in this.devices" :key="item" :value="item" :selected="(item===-1)">
        {{ "Node " + (item === -1 ? "all" : item) }}
      </option>
    </select>
    <MinMaxAvg title="Temperature" unit="°C" :color="this.TEMP_COLOR"
               v-bind:data="this.received_data.map(x=>x.temperature)"></MinMaxAvg>
    <MinMaxAvg title="Humidity" unit="%" :color="this.HUMIDITY_COLOR"
               v-bind:data="this.received_data.map(x=>x.humidity)"></MinMaxAvg>
    <MinMaxAvg title="Soil moisture" unit="%" :color="this.SOIL_COLOR"
               v-bind:data="this.received_data.map(x=>x.soil)"></MinMaxAvg>
    <br>
    <div>
      <p><b>Last update: </b>{{ this.formatTimestamp(this.last_update.timestamp) }}</p>
      <table style="margin: auto">
        <tr>
          <td>
            <p><b>Water level: </b>{{ this.last_update.water_level }}%</p>
          </td>
          <td>
            <p><b>Window: </b>{{ this.last_update.servo === 180 ? 'open' : 'close' }}</p>
          </td>
          <td>
            <p><b>Pump state: </b>{{ this.last_update.pump === 1 ? 'on' : 'off' }}</p>
          </td>
        </tr>

        <tr>
          <td>
            <p><b>Temperature: </b>{{ this.last_update.temperature }} °C</p>
          </td>
          <td>
            <p><b>Humidity: </b>{{ this.last_update.humidity }} %</p>
          </td>
          <td>
            <p><b>Soil moisture: </b>{{ this.last_update.soil }} %</p>
          </td>
        </tr>
      </table>
    </div>

    <ButtonPanel :device="this.selectedDevice"></ButtonPanel>
    <div v-if="this.received_data.length>0" class="flex_container">

      <my-graph class="graph" v-bind:values="this.received_data.map(x=>x.temperature)"
                v-bind:ts="this.received_data.map(x=>x.timestamp)" name="Temperature" :color="this.TEMP_COLOR">
      </my-graph>

      <my-graph class="graph" v-bind:values="this.received_data.map(x=>x.humidity)"
                v-bind:ts="this.received_data.map(x=>x.timestamp)" name="Humidity" :color="this.HUMIDITY_COLOR">
      </my-graph>

      <my-graph class="graph" v-bind:values="this.received_data.map(x=>x.soil)"
                v-bind:ts="this.received_data.map(x=>x.timestamp)" name="Soil moisture" :color="this.SOIL_COLOR">
      </my-graph>

    </div>
    <SensorSelector/>
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
import SensorSelector from "./SensorSelector";

export default {
  name: "Sensors",
  components: {
    SensorSelector,
    ButtonPanel,
    MyGraph,
    MinMaxAvg,
    Loading
  },
  data: function () {
    return {
      fetch_data_url: "https://bvj926atxc.execute-api.us-east-1.amazonaws.com/default/getDevicesID",
      received_data: [],
      last_update: {},
      TEMP_COLOR: "#ef476f",
      HUMIDITY_COLOR: "#ffd166",
      SOIL_COLOR: "#06d6a0",
      isLoading: true,
      devices: [4, 5, 6],
      selectedDevice: 5
    }
  },
  mounted() {
    this.loadData();
    setInterval(this.loadData, 10000);
  },
  watch: {
    selectedDevice: function () {
      this.loadData()
    }
  },
  methods: {
    formatTimestamp(ts) {
      return moment(ts).format("LLL")
    },
    updateLastUpdate() {
      const ordered_received_data = this.received_data.sort((x, y) => {
        if (x.timestamp < y.timestamp) return -1;
        else if (x.timestamp > y.timestamp) return 1;
        else return 0
      })
      this.last_update = ordered_received_data[ordered_received_data.length - 1];
    },
    loadData() {
      axios.get(this.fetch_data_url + "?device=" + this.selectedDevice)
          .then(d => {

            this.received_data = d.data.Items
            this.updateLastUpdate()
          })
          .finally(() => {
            this.isLoading = false
          })
          .catch(err => {
            console.log(err)
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