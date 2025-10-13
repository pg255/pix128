const pix = {
	loader: {
		start() {
			pix.start();
		}
	},
	start() {
		const el = document.createElement('div');
		el.textContent = "Hello world!";
		el.id = "title";
		document.body.appendChild(el);
	}
}