<template>
  <div id="app">
    <Title msg="RIOT Greenhouse"/>
    <Sensor/>

  </div>
</template>

<script>
import Title from './components/Title.vue'
import Sensor from "./components/Sensors";

export default {
  name: 'App',
  components: {
    Title, Sensor
  }
}
</script>

<style>
button {
  margin: 10px;
}

#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 15px;
}
</style>
