inlets = 1
outlets = 1

// ~- listener states
// missing, watching, dancing, experiencing

// ~- overall states
// idle, stars, firefly, converging
// oneMemory, twoMemories, threeMemories, fadeMemory
// finale

let state = "idle"
let previousState = "idle"
let updateTime = 0.033
let progressTimer = 0.0
let progressScalarValue = 0.0
let stateTime = 0.0

const convergingTime = 5.0
const oneMemoryTime = 10.0
const twoMemoriesTime = 10.0
const threeMemoriesTime = 10.0
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
		case "converging":
			progressTimer += updateTime
			progressScalarValue = progressTimer / convergingTime
			stateTime = convergingTime

			if (progressTimer > convergingTime) {
				state = "oneMemory"
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
		case "threeMemories" :
			progressTimer += updateTime
			progressScalarValue = progressTimer / threeMemoriesTime
			stateTime = threeMemoriesTime
			
			if (progressTimer > threeMemoriesTime) {
				state = "finale"
				progressTimer = 0
			}
	
			break
		case "finale" :
			progressTimer += updateTime
			progressScalarValue = progressTimer / finaleTime
			stateTime = finaleTime

			if (progressTimer > finaleTime) {
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
		case "watching":
			checkWatching(state)
			break
		case "dancing":
			checkDancing(state)
			break
		case "experiencing":
			checkExperiencing(state)
			break
	}
}


function checkMissing(oldState) {
	switch(oldState) {
  		case "idle":
			setState("idle")
    		break
  		case "firefly":
			setState("idle")
    		break
		case "stars":
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
		case "finale":
			setState("fadeMemory")
    		break
	} 
}

function checkWatching(oldState) {
	switch(oldState) {
  		case "idle":
			setState("stars")
    		break
		case "firefly":
			setState("stars")
    		break
	} 
}

function checkDancing(oldState) {
	switch(oldState) {
  		case "idle":
			setState("firefly")
    		break
		case "stars":
			setState("firefly")
    		break
		case "converging":
			setState("firefly")
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
		case "finale":
			setState("fadeMemory")
    		break
	} 
}

function checkExperiencing(oldState) {
	switch(oldState) {
  		case "idle":
			progressTimer = 0.0
			setState("converging")
    		break
		case "stars":
			progressTimer = 0.0
			setState("converging")
    		break
		case "firefly":
			progressTimer = 0.0
			setState("converging")
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
	post(state)
}