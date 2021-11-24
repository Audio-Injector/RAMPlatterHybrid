#! /usr/bin/env node
var libSwigCNodejs = require('../swig/.libs/libSwigCNodejs');
console.log('constructing Audio<int>')
let audio = new libSwigCNodejs.intAudio;
console.log('constructing PlatterAudio<int, 48000>')
let platterAudio = new libSwigCNodejs.intPlatterAudio48k;
