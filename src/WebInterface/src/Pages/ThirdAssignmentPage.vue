<template>
  <div>
    <loading :active.sync="isLoading"
             :can-cancel="false"
             :is-full-page="true"></loading>
    <!--
    <MinMaxAvg title="Temperature" unit="°C" :color="this.TEMP_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.temperature)"></MinMaxAvg>
    <MinMaxAvg title="Humidity" unit="%" :color="this.HUMIDITY_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.humidity)"></MinMaxAvg>
    <MinMaxAvg title="Soil moisture" unit="%" :color="this.SOIL_COLOR"
               v-bind:data="this.$data.received_data.map(x=>x.soil)"></MinMaxAvg>
               -->
    <h3 :style="{color:this.TEMP_COLOR}"><b>Temperature</b></h3>
    <p>
      <b>min:</b> {{ this.received_data.temp_min }} °C
      <b>avg:</b> {{ this.received_data.temp_max }} °C
      <b>max:</b> {{ this.received_data.temp_avg }} °C
    </p>

    <h3 :style="{color:this.HUMIDITY_COLOR}"><b>Humidity</b></h3>
    <p>
      <b>min:</b> {{ this.received_data.hum_min }} %
      <b>avg:</b> {{ this.received_data.hum_max }} %
      <b>max:</b> {{ this.received_data.hum_avg }} %
    </p>

    <h3 :style="{color:this.SOIL_COLOR}"><b>Soil moisture</b></h3>
    <p>
      <b>min:</b> {{ this.received_data.soil_min }} %
      <b>avg:</b> {{ this.received_data.soil_max }} %
      <b>max:</b> {{ this.received_data.soil_avg }} %
    </p>
    <br>
    <p>
      <b>Last update: </b>{{ this.formatData(this.received_data.last_update) }}
    </p>

  </div>
</template>

<script>
import axios from "axios";
import moment from "moment"
// Import component
import Loading from 'vue-loading-overlay';
// Import stylesheet
import 'vue-loading-overlay/dist/vue-loading.css';

export default {
  name: "ThirdAssignmentPage",
  components: {
    Loading
  },
  data() {
    return {
      isLoading: true,
      received_data: [],
      TEMP_COLOR: "#ef476f",
      HUMIDITY_COLOR: "#ffd166",
      SOIL_COLOR: "#06d6a0",
    }
  },
  methods: {
    loadData() {
      axios.get("https://mesgqwybah.execute-api.us-east-1.amazonaws.com/default/getAggregatedValue")
          .then(d => {
            this.received_data = d.data.Items[0]
            this.isLoading = false
          })
    },
    formatData(data) {
      return moment(data).format("LLL")
    }
  },
  mounted() {
    this.loadData()
    setInterval(this.loadData, 60000);
  }
}
</script>

<style scoped>

</style>