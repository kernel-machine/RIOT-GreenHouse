<template>
  <div id="app">
    <Title msg="RIOT Greenhouse"/>
    <Sensor/>
    <github-button href="https://github.com/kernel-machine/RIOT-GreenHouse" data-size="large">Open on GitHub
    </github-button>
  </div>
</template>

<script>
import Title from './components/Title.vue'
import Sensor from "./components/Sensors";
import GithubButton from 'vue-github-button'

export default {
  name: 'App',
  components: {
    Title, Sensor, GithubButton
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
