<template>
  <div id="app">
    <Title msg="RIOT Greenhouse"/>
    <p>Select the assignment web interface</p>
    <select v-model="homeworkSelected">
      <option value="1">First Assignment</option>
      <option value="2">Second Assignment</option>
      <option value="3">Third Assignment</option>
    </select>
    <FirstAssignmentPage v-if="homeworkSelected==1"/>
    <SecondAssignmentPage v-if="homeworkSelected==2"/>
    <ThirdAssignmentPage v-if="homeworkSelected==3"/>
    <github-button href="https://github.com/kernel-machine/RIOT-GreenHouse" data-size="large">Open on GitHub
    </github-button>
  </div>
</template>

<script>
import Title from './components/Title.vue'
import FirstAssignmentPage from "./Pages/FirstAssignmentPage";
import SecondAssignmentPage from "./Pages/SecondAssignmentPage";
import ThirdAssignmentPage from "./Pages/ThirdAssignmentPage";
import GithubButton from 'vue-github-button'

export default {
  name: 'App',
  components: {
    FirstAssignmentPage,
    SecondAssignmentPage,
    ThirdAssignmentPage,
    Title,  GithubButton,
  },
  data() {
    return {
      homeworkSelected: 1
    }
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
