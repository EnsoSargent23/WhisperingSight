// Starting the game

function startGame(){

    let mainfield = $("#mainfield");

    mainfield.html('<div class="container">    <div class="row">        <div class="col-md-2"></div>        <div class="col-md-8">            <div class="player-box">Player 1</div>        </div>        <div class="col-md-2"></div>    </div>    <div class="row">        <div class="col-md-2">            <div class="player-box">Player 4</div>        </div>        <div class="col-md-8">            <div class="container">                <div class="row">                    <div class="col-md-6">                        <div class="player-box">Mitte</div>                    </div>                    <div class="col-md-6">                        <div class="player-box">Dealer</div>                    </div>                </div>            </div>        </div>        <div class="col-md-2">            <div class="player-box">Player 2</div>        </div>    </div>    <div class="row">        <div class="col-md-2"></div>        <div class="col-md-8">            <div class="player-box">Player 3</div>        </div>        <div class="col-md-2"></div>    </div></div>');
}

const socket = new WebSocket('ws://192.168.178.24:3001');

socket.addEventListener('open', (event) => {
    console.log('WebSocket-Verbindung hergestellt');
});

socket.addEventListener('message', (event) => {
    const messageDiv = document.getElementById('message');
    messageDiv.innerHTML = `${event.data}`;
    
    
});

socket.addEventListener('close', (event) => {
    console.log('WebSocket-Verbindung getrennt');
});


// Funktion zum Vorlesen der Nachricht
function speakMessage() {
    // Verwenden Sie die SpeechSynthesis-API, um die Nachricht vorzulesen
    
    let text = $('#message').html();
    
    
    const synth = window.speechSynthesis;
    const utterance = new SpeechSynthesisUtterance(text);
    synth.speak(utterance);
    utterance.onend = function (){
        console.log("funktioniert");
    }
    utterance.onerror= function (){
        alert("fehlschlag");
    }
}
