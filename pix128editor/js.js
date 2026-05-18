var currentTop = "picture";
var currentZoom = 1;
var isMouseDown = false;

var imageWidth, imageHeight;

function switchTop(id) {
	document.getElementById(currentTop).style.display = "none";
	document.getElementById(id).style.display = "block";
	currentTop = id;
	drawOverlay();
}

function resize() {
	document.getElementById("overlay").width = window.innerWidth - 75;
	document.getElementById("overlay").height = window.innerHeight - 75;
}

document.getElementById("fileUpload").onchange = function (e) {
	var img = new Image();
	img.onload = function () {
		let canvas = document.getElementById("canvas");
		setSize(this.width, this.height);
		let ctx = canvas.getContext("2d");
		ctx.drawImage(img, 0, 0);
	};
	img.src = URL.createObjectURL(this.files[0]);
};

function setSize(w, h) {
	document.getElementById("canvas").width = w;
	document.getElementById("canvas").height = h;
	document.getElementById("canvas").style.width = (w * currentZoom) + "px";
	document.getElementById("canvas").style.height = (h * currentZoom) + "px";
	document.getElementById("overlay").width = (w * currentZoom);
	document.getElementById("overlay").height = (h * currentZoom);
	document.getElementById("overlay").style.width = (w * currentZoom) + "px";
	document.getElementById("overlay").style.height = (h * currentZoom) + "px";
	document.getElementById("canvasWidth").value = w;
	document.getElementById("canvasHeight").value = h;
	imageWidth = w;
	imageHeight = h;
}

function zoomImage(zoom) {
	document.getElementById("canvas").style.width = (imageWidth * zoom) + "px";
	document.getElementById("canvas").style.height = (imageHeight * zoom) + "px";
	document.getElementById("overlay").style.width = (imageWidth * zoom) + "px";
	document.getElementById("overlay").style.height = (imageHeight * zoom) + "px";
	document.getElementById("overlay").width = (imageWidth * zoom);
	document.getElementById("overlay").height = (imageHeight * zoom);
	document.getElementById("canvasZoom").value = zoom;
	currentZoom = zoom;
}

function download() {
	let canvas = document.getElementById("canvas");

	const dataURL = canvas.toDataURL("image/png");

	const link = document.createElement("a");
	link.href = dataURL;
	link.download = prompt("Select name for the file:");

	document.body.appendChild(link); // Required for Firefox
	link.click();
	document.body.removeChild(link); // Clean up
}

function click(e) {

}

function mouseMove(e) {
	if (currentTop == "draw" && isMouseDown) {
		draw(e.offsetX / currentZoom, e.offsetY / currentZoom, e.pressure);
	}
}

function draw(x, y, pressure) {
	let size = parseFloat(document.getElementById("drawSize").value);
	if (document.getElementById("drawPressure").value == "without") {
		pressure = 1;
	}
	let opacity = parseFloat(document.getElementById("drawOpacity").value) * pressure;
	let color = document.getElementById("drawColor").value;
	var ctx = document.getElementById("canvas").getContext("2d");
	let mode = document.getElementById("drawMode").value;
	if (mode == "square") {
		ctx.globalAlpha = opacity;
		ctx.fillStyle = color;
		ctx.fillRect(Math.round(x - (size / 2)), Math.round(y - (size / 2)), size, size);
		ctx.globalAlpha = 1.0;
	} else if (mode == "circle") {
		ctx.beginPath();
		ctx.globalAlpha = opacity;
		ctx.arc(Math.round(x), Math.round(y), size / 2, 0, 2 * Math.PI);
		ctx.fillStyle = color;
		ctx.fill();
		ctx.globalAlpha = 1.0;
	} else if (mode == "erase") {
		ctx.globalAlpha = opacity;
		ctx.clearRect(Math.round(x - (size / 2)), Math.round(y - (size / 2)), size, size);
		ctx.globalAlpha = 1.0;
	} 
}

document.onkeypress = function (e) {
    e = e || window.event;
	console.log(e.keyCode);
	switch (e.keyCode) {
		case 119:
			moveCursor(cursorX, cursorY - 1);
			break;
		case 115:
			moveCursor(cursorX, cursorY + 1);
			break;
		case 97:
			moveCursor(cursorX - 1, cursorY);
			break;
		case 100:
			moveCursor(cursorX + 1, cursorY);
			break;
		case 44:
			setStartCords(cursorX, cursorY);
			break;
		case 46:
			setEndCords(cursorX, cursorY);
			break;
		case 45:
			addData();
			break;
		case 109: 
			colliderData.pop(); // remove last element
			document.getElementById("colliderData").value = JSON.stringify(colliderData);
			drawOverlay();
		case 32:
			setEndCords(cursorX, cursorY);
			addData();
			setStartCords(cursorX, cursorY);
	}
};

var cursorX, cursorY;
var colliderData, colliderStartX, colliderStartY, colliderEndX, colliderEndY; 

function moveCursor(x, y) {
	cursorX = x;
	cursorY = y;
	document.getElementById("cursorX").value = x;
	document.getElementById("cursorY").value = y;
	drawOverlay();
}

function setStartCords(x, y) {
	colliderStartX = x;
	colliderStartY = y;
	document.getElementById("startX").value = x;
	document.getElementById("startY").value = y;
	drawOverlay();
}

function setEndCords(x, y) {
	colliderEndX = x;
	colliderEndY = y;
	document.getElementById("endX").value = x;
	document.getElementById("endY").value = y;
	drawOverlay();
}

function setData(dat) {
	colliderData = dat;
	document.getElementById("colliderData").value = JSON.stringify(dat);
	drawOverlay();
}

function addData() {
	let dat = [parseFloat(colliderStartX), parseFloat(colliderStartY), parseFloat(colliderEndX), parseFloat(colliderEndY)];
	if ((!dat[0] && dat[0] != 0) || (!dat[1] && dat[1] != 0) || (!dat[2] && dat[2] != 0) || (!dat[3] && dat[3] != 0)) {
		console.log(dat[0], dat[1], dat[2], dat[3])
		return;
	}
	colliderData.push(dat);
	document.getElementById("colliderData").value = JSON.stringify(colliderData);
	drawOverlay();
	setStartCords(undefined, undefined);
	setEndCords(undefined, undefined);
}

function drawOverlay() {
	let ctx = document.getElementById("overlay").getContext("2d");
	ctx.clearRect(0, 0, (imageWidth * currentZoom), (imageHeight * currentZoom));
	if (currentTop == "collider") {
		if (imageWidth < cursorX || imageHeight < cursorY) {
			moveCursor(0, 0);
		}
		for (let i in colliderData) {
			let ctx = document.getElementById("overlay").getContext("2d");
			ctx.beginPath();
			ctx.moveTo(colliderData[i][0] * currentZoom, colliderData[i][1] * currentZoom);
			ctx.lineTo(colliderData[i][2] * currentZoom, colliderData[i][3] * currentZoom);
			ctx.lineWidth = 5;
			ctx.strokeStyle = "#00ffff";
			ctx.stroke();
		}
		let ctx = document.getElementById("overlay").getContext("2d");
		ctx.beginPath();
		ctx.moveTo(colliderStartX * currentZoom, colliderStartY * currentZoom);
		ctx.lineTo(colliderEndX * currentZoom, colliderEndY * currentZoom);
		ctx.lineWidth = 5;
		ctx.strokeStyle = "#ffff00";
		ctx.stroke();
		ctx = document.getElementById("overlay").getContext("2d");
		ctx.fillStyle = "#ff0000";
		ctx.fillRect(colliderStartX * currentZoom, colliderStartY * currentZoom, 10, 10);
		ctx.fillStyle = "#ff8800";
		ctx.fillRect(colliderEndX * currentZoom, colliderEndY * currentZoom, 10, 10);
		ctx.fillStyle = "#00ff00";
		ctx.fillRect(cursorX * currentZoom, cursorY * currentZoom, 10, 10);
	}
}

setSize(640, 400);
moveCursor(5, 5);
setStartCords(undefined, undefined);
setEndCords(undefined, undefined);
setData([]);
zoomImage(4);

function zoomImage(lvl) {
	
}