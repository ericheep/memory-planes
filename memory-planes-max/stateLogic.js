inlets = 1
outlets = 1

// ~- listener states
// missing, present

// ~- overall states
// idle,
// oneMemory, twoMemories, threeMemories, fadeMemory

let state = "idle"
let previousState = "idle"
let updateTime = 0.033
let progressTimer = 0.0
let progressScalarValue = 0.0
let stateTime = 0.0

const oneMemoryTime = 30.0
const twoMemoriesTime = 20.0
const threeMemoriesTime = 20.0
const finaleTime = 10.0
const fadeMemoryTime = 10.0

function update() {
	switch(state) {
		case "fadeMemory":
			progressTimer += updateTime
			progressScalarValue = progressTimer / fadeMemoryTime
			stateTime = fadeMemoryTime
			
			if (progressTimer > fadeMemoryTime) {
				state = "idle"
				progressTimer = 0
			}
	
			break
		case "oneMemory":
			progressTimer += updateTime
			progressScalarValue = progressTimer / oneMemoryTime
			stateTime = oneMemoryTime

			if (progressTimer > oneMemoryTime) {
				state = "twoMemories"
				progressTimer = 0
			}
	
			break
		case "twoMemories" :
			progressTimer += updateTime
			progressScalarValue = progressTimer / twoMemoriesTime
			stateTime = twoMemoriesTime
			
			if (progressTimer > twoMemoriesTime) {
				state = "threeMemories"
				progressTimer = 0
			}
	
			break
	}	
	
		
	outlet(0, state, progressTimer, progressScalarValue, stateTime)
}

function setListenerState(listenerState) {
	switch(listenerState) {
		case "missing":
			checkMissing(state)
			break
		case "present":
			checkPresent(state)
			break
	}
}


function checkMissing(oldState) {
	switch(oldState) {
  		case "idle":
			setState("idle")
    		break
		case "oneMemory":
			setState("fadeMemory")
    		break
		case "twoMemories":
			setState("fadeMemory")
    		break
		case "threeMemories":
			setState("fadeMemory")
    		break
	} 
}


function checkPresent(oldState) {
	switch(oldState) {
  		case "idle":
			progressTimer = 0.0
			setState("oneMemory")
    		break
		case "fadeMemory":
			progressTimer = 0.0
			setState(previousState)
			break
	} 
}


function setState(newState) {
	previousState = state
	state = newState
}