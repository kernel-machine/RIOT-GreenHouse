<template>
  <div>
    <button v-on:click="togglePump(1)" class="myButton">Turn on pump</button>
    <button v-on:click="togglePump(0)" class="myButton">Turn off pump</button>
    <br>
    <button v-on:click="toggleWindow(1)" class="myButton">Open window</button>
    <button v-on:click="toggleWindow(0)" class="myButton">Close window</button>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: "ButtonPanel",
  methods: {
    togglePump(state) {
      axios.post("https://s68cxgvow4.execute-api.us-east-1.amazonaws.com/default/publishData", {
        "name": "pump",
        "value": state
      })
    },
    toggleWindow(state) {
      axios.post("https://s68cxgvow4.execute-api.us-east-1.amazonaws.com/default/publishData", {
        "name": "servo",
        "value": state ? 180 : 0
      }).then(d => {
        console.log(d.data)//dati ricevuti
      })
    }
  }
}
</script>

<style scoped>
.myButton {
  background-color: #06d6a0;
  border-radius: 10px;
  border: 2px solid #029770;
  display: inline-block;
  cursor: pointer;
  color: #ffffff;
  font-family: Arial;
  font-size: 17px;
  padding: 14px 32px;
  text-decoration: none;
}

.myButton:hover {
  background-color: #ffd166;
  border: 2px solid #faba27;
}

.myButton:active {
  position: relative;
  top: 1px;
}

</style>