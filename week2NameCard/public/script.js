let qrButton;

function setup(event) {
    qrButton = document.getElementById("button");
    qrButton.addEventListener('click', generateQrCode)
}

function generateQrCode(event) {
    console.log("button clicked");
    // get the div element for the QR code image:
    let qrDiv = document.getElementById('qrCode');
     // get th text of the text field:
  let qrText = "http://159.89.50.99:8080/formdata";
    // make the QR code:
    let qr = qrcode(0, 'L');
    qr.addData(qrText);
    qr.make();
    // create an image from it:
    let qrImg = qr.createImgTag(5,0, "qr code of " + qrText);
    // add it to the div:
    qrDiv.innerHTML = qrImg;
    // if qr code is generated, change the button value to "regenerate"
}

window.addEventListener('DOMContentLoaded', setup);
