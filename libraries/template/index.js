function addMessage(msg) {
	const el = document.createElement('div');
	el.textContent = msg;
	el.classList.add("msg")
	document.body.appendChild(el);
}