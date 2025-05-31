inlets = 1
outlets = 2

var echoes = []
var numEchoes = 5
var duration = 1000


function setEchoes(n, d) {
	numEchoes = n
	duration = d
	
	calculateEchoes(numEchoes, duration)
	outlet(1, echoes)
}

function calculateEchoes(numEchoes, duration) {
  var inverseN = 1.0 / numEchoes
  var beatTime = duration * inverseN

  var y = 0
  var echoLength = 0
  echoes = []

  // calculates duration of each echo
  for (var i = 1; i < numEchoes + 1; i++) {
    var beat = Math.pow(Math.pow((1 + numEchoes),(inverseN)),(i)) - 1
    var x = beat * beatTime

    var echoLength = x - y
    var y = beat * beatTime

	echoes.push(echoLength)
  }
}

function msg_int(idx) {
	if (idx >= 0 && idx < numEchoes) {
		outlet(0, echoes[idx])
	}
}

function bang() {
	outlet(0, echoes)
}