inlets = 1
outlets = 1

function list() {
	const points = arrayfromargs(arguments);
	const n = points.length / 2
	
	if (n < 2) return; 
	
	let sumX = 0, sumY = 0, sumXY = 0, sumXX = 0

	for (var i = 0; i < points.length; i += 2) {
		const x = points[i]
		const y = points[i + 1]
		
		sumX += x
		sumY += y
		sumXY += x * y
		sumXX += x * x
	}
	
	const slopeNumerator = n * sumXY - sumX * sumY;
	const slopeDenominator = n * sumXX - sumX * sumX;

	if (slopeDenominator === 0) {
		const y1 = points[1];
		const y2 = points[points.length - 1];
		const angleRad = (y2 > y1) ? Math.PI / 2 : -Math.PI / 2;
		
		outlet(0, angleRad, 1.0);
		return;
	}

	const m = slopeNumerator / slopeDenominator;
	const b = (sumY - m * sumX) / n;

	const x1 = points[0];
	const x2 = points[points.length - 2];

	const y1 = m * x1 + b;
	const y2 = m * x2 + b;

	const dx = x2 - x1;
	const dy = y2 - y1;

	const angleRad = Math.atan2(dy, dx);

	let meanY = sumY / n;
	let ssTot = 0;
	let ssRes = 0;

	for (var i = 0; i < points.length; i += 2) {
		const x = points[i];
		const y = points[i + 1];
		const yPred = m * x + b;

		ssTot += Math.pow(y - meanY, 2);
		ssRes += Math.pow(y - yPred, 2);
	}

	const r2 = ssTot === 0 ? 1.0 : 1.0 - (ssRes / ssTot);
	const confidence = Math.max(0, Math.min(1, r2)); 

  	outlet(0, angleRad, Math.hypot(dx, dy), confidence)
}