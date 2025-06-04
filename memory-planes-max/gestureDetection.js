inlets = 1
outlets = 1

const TWO_PI = Math.PI * 2.0
const PI = Math.PI

const directions = [0, PI * 0.5, PI, PI * 1.5]

function wrapToTwoPi(angle) {
	return (angle % TWO_PI + TWO_PI) % TWO_PI;
}

function list() {
	const data = arrayfromargs(arguments);
	
	const theta = wrapToTwoPi(data[0])
	const length = data[1]
	
	if (length < 0.06) {
		outlet(0, "tap")
		return
	}
	
	let closestIndex = 0
	let closestDistance = TWO_PI
	
	for (var i = 0; i < directions.length; i++) {
		const distance = Math.abs(theta - directions[i])
		if (distance < closestDistance) {
			closestDistance = distance
			closestIndex = i
		}
	}
	
	outlet(0, closestIndex)
}