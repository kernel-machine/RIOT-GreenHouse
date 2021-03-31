<template>
  <vue-apex-charts ref="realtimeChart" type="line" height="350" :options="chartOptions"
                   :series="series"></vue-apex-charts>
</template>

<script>
import VueApexCharts from 'vue-apexcharts'
import moment from "moment"

export default {
  name: "MyGraph",
  components: {
    VueApexCharts
  },
  props: ['values', 'ts', 'name', 'val','color'],
  data() {
    return {
      chartOptions: {
        chart: {
          height: 350,
          type: 'line'
        },
        stroke: {
          curve: 'stepline'
        },
        title: {
          text: this.name,
          align: 'left'
        },
        grid: {
          row: {
            colors: ['#f3f3f3', 'transparent'], // takes an array which will be repeated on columns
            opacity: 0.5
          },
        },
        xaxis: {
          categories: [],
        },
        colors: [this.color],
      },
      series: [{
        name: this.name,
        data: []
      }],

    }
  },
  methods: {
    updateValues(data) {
      this.series = [{
        data: data
      }]
    },
    updateTs(data) {
      const labels = data.map(x => this.formatTimestamp(x.timestamp));

      const xaxis = {
        categories: labels
      }
      this.chartOptions = {
        xaxis: xaxis
      };
    },
    formatTimestamp(ts) {
      return moment(ts).format("LLL")
    },
  },
  watch: {
    'values': function () {
      this.updateValues(this.values)
    },
    'ts': function () {
      this.updateTs(this.ts)
    }
  }
}
</script>

<style scoped>

</style>