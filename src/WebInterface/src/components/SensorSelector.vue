<template>
  <div>
    <loading :active.sync="isLoading"
             :can-cancel="false"
             :is-full-page="false"></loading>
    <a>Aggregated values by device</a>

    <MinMaxAvg title="Temperature" unit="°C" color="#ef476f"
               v-bind:data="received_data.map(x=>x.temperature)"></MinMaxAvg>
    <MinMaxAvg title="Humidity" unit="%" color="#e05a10"
               v-bind:data="received_data.map(x=>x.humidity)"></MinMaxAvg>
    <MinMaxAvg title="Soil moisture" unit="%" color="#06d6a0"
               v-bind:data="received_data.map(x=>x.soil)"></MinMaxAvg>
    <MinMaxAvg title="Soil moisture" unit="%" color="#fed67b"
               v-bind:data="received_data.map(x=>x.water_level)"></MinMaxAvg>
    <a>Select a sensor </a>
    <select v-model="selectedSensor" v-if="received_data.length>0">
      <option value="1" selected="true">Temperature</option>
      <option value="2">Humidity</option>
      <option value="3">Soil</option>
      <option value="4">Water level</option>
    </select>
    <div class="flex_container">
      <my-graph class="graph" v-bind:values="received_data.filter(x=>x.device_id==4).map(mapBySensor)"
                v-bind:ts="received_data.filter(x=>x.device_id==4).map(x=>x.timestamp)" name="Device 4"
                color="#ef476f">
      </my-graph>

      <my-graph class="graph" v-bind:values="received_data.filter(x=>x.device_id==5).map(mapBySensor)"
                v-bind:ts="received_data.filter(x=>x.device_id==5).map(x=>x.timestamp)" name="Device 5"
                color="#e05a10">
      </my-graph>

      <my-graph class="graph" v-bind:values="received_data.filter(x=>x.device_id==6).map(mapBySensor)"
                v-bind:ts="received_data.filter(x=>x.device_id==6).map(x=>x.timestamp)" name="Device 6"
                color="#fed67b">
      </my-graph>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import MinMaxAvg from "./MinMaxAvg";
import MyGraph from "@/components/MyGraph";
import Loading from "vue-loading-overlay";

export default {
  name: "SensorSelector",
  components: {MinMaxAvg, MyGraph, Loading},
  data() {
    return {
      selectedSensor: 1,
      received_data: [],
      received_data_by_device: [],
      isLoading: true
    }
  },
  watch: {
    selectedSensor: function () {
      this.selectedSensor = Number(this.selectedSensor)
    }
  },
  mounted() {
    this.loadData()
    setInterval(this.loadData, 60 * 1000)
  },
  methods: {
    loadData() {
      axios.get("https://a88wox1sp3.execute-api.us-east-1.amazonaws.com/default/getSensorValues")
          .then(d => {
            this.received_data = d.data.Items
            this.isLoading = false
          })
    },
    mapBySensor(x) {
      if (this.selectedSensor == 1)
        return x.temperature
      else if (this.selectedSensor == 2)
        return x.humidity
      else if (this.selectedSensor == 3)
        return x.soil
      else
        return x.water_level
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
</style>