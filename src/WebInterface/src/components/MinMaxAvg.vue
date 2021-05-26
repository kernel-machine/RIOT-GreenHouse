<template>
  <div v-if="this.data.length>0">
    <h3 :style="{color:this.color}"><b>{{ this.title }}</b></h3>
    <p>
      <b>min:</b> {{ this.minValue(this.data) }} {{ this.unit }}
      <b>avg:</b> {{ this.avgValue(this.data) }} {{ this.unit }}
      <b>max:</b> {{ this.maxValue(this.data) }} {{ this.unit }}
    </p>
  </div>
</template>

<script>
export default {
  name: "MinMaxAvg",
  props: ['data', 'title', 'unit', 'color'],
  methods: {
    minValue(datas) {
      return datas.reduce((acc, current) => {
        return Math.min(acc, current)
      })
    },
    avgValue(datas) {
      let avg = datas.reduce((acc, current) => {
        return acc + current
      }) / datas.length

      return Math.floor(avg)
    },
    maxValue(datas) {
      return datas.reduce((acc, current) => {
        return Math.max(acc, current)
      })
    },
  }
}
</script>

<style>

</style>