const checkTextArea = document.getElementById("value");
const infoTextArea = document.getElementById("info");
const sendButton = document.getElementById("send");
const openButton = document.getElementById("open");
const resultImage = document.getElementById("result-image");
const resultText = document.getElementById("result-text");

const serverIp = "18.218.69.65";
const regExp = /[^(\d+(.\d+)?)]/g;

const EMPTY_STRING = "";
const MIN_WORDS_COUNT = 3;
const MAX_PERCENTAGE = 100;
const CLASS_BTN_DISABLED = "btn_disabled";
const SEND_BTN_TITLE = `Enter at least ${MIN_WORDS_COUNT} word!`;

function disableButton(isDisabled) {
	sendButton.disabled = isDisabled;
	isDisabled ? sendButton.classList.add(CLASS_BTN_DISABLED) : sendButton.classList.remove(CLASS_BTN_DISABLED);
	sendButton.title = isDisabled ? SEND_BTN_TITLE : EMPTY_STRING;
}

function showResult(answerNumber) {
	resultImage.classList.add("partial-opacity");
	resultText.innerHTML = `This text is ${answerNumber}% unique`;
	resultText.classList.add("full-opacity");
}

function checkWordsCount(event) {
	const wordsArray = event.target.value.split(" ").filter( str => str.length > 0 );
	const wordsCount = wordsArray.length;

	if (wordsCount >= MIN_WORDS_COUNT) {
		disableButton(false);
		
		if (event.code === "Enter") {			
			sendButton.click();
		}
	} else {
		disableButton(true);
	}
}

async function requestResult() {
  const payload = "name=" + encodeURIComponent(checkTextArea.value);

	try {
		const response = await fetch(`http://${serverIp}:8080/cgi-bin/script.cgi`, {
				method: "POST",
				headers: {"Content-Type": "application/x-www-form-urlencoded"},
				body: payload
		});

		const text = await response.text();
		const answerNumber = MAX_PERCENTAGE - parseInt( text.replace(regExp, EMPTY_STRING) );

		showResult(answerNumber);
	} catch (e) {
		throw new Error(e.message);
	}
}

async function requestOriginalText() {
  infoTextArea.innerText = EMPTY_STRING;

	try {
		const response = await fetch(`http://${serverIp}:8080/cgi-bin/text.cgi`);
		const text = await response.text();
		
		infoTextArea.innerHTML = text;
		infoTextArea.style.color = 'black';
	} catch (e) {
		throw new Error(e.message);
	}
}

checkTextArea.addEventListener("keyup", (e) => checkWordsCount(e));
sendButton.addEventListener("click", requestResult);
openButton.addEventListener("click", requestOriginalText);

disableButton(true);