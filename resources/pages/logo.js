//var domain = 'http://192.168.2.110';
var domain = '';
var data;
var ledToSet;

/**
 * Sends a request to the logo to update it.
 */
function sendRequestToLogo(url) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            data = JSON.parse(this.responseText);
            console.log(data);
            updatePage();
        } else {
            console.log("ERROR response with status " + this.status);
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
}

/**
 * Builds the color parameter for a HTTP request to the logo.
 */
function buildColorParameter() {
    var colors = "";
    for (var i = 0; i < data.leds; i++) {
        colors += data.colors[i];
        if (i < data.leds - 1) {
            colors += ',';
        }
    }
    return 'colors=' + colors;
}

/**
 * Changes the speed of the current animation.
 */
function updateSpeed(button) {
    var url = domain + data.path + '?';

    if (data.usage == 'Single' || data.usage == 'Strip') {
        url += buildColorParameter() + '&';
    }

    var speed = data.speed;

    if (button == '--') {
        speed -= 10;
    } else if (button == '-') {
        speed -= 5;
    } else if (button == '+') {
        speed += 5;
    } else if (button == '++') {
        speed += 10;
    }

    if (speed < 0) {
        speed = 0;
    } else if (speed > 100) {
        speed = 100;
    }

    url += 'speed=' + speed;
    sendRequestToLogo(url);
};

/**
 * Updates the color settings of the current animation
 */
function updateColorList() {
    sendRequestToLogo(domain + data.path + '?' + buildColorParameter());
}

/**
 * Updates the UI when a scene was selected.
 */
function changeSelectedScene() {
    var scene = $('#scSceneSelect option:selected').text();

    if (scene != 'Select a scene...') {
        $('#scSceneButtonRemove').show();
        $('#scSceneButtonEnable').show();
    } else {
        $('#scSceneButtonRemove').hide();
        $('#scSceneButtonEnable').hide();
    }
}

function loadScene(scene) {
    if (!scene) {
        scene = $('#scSceneSelect option:selected').text();
    }
    sendRequestToLogo(domain + '/led/scene/set' + '?name=' + scene);
}

/**
 * Adds a new scene.
 */
function addScene() {
    var sceneName = $('#scSceneModalAddName').val();
    var errorText = '';
    if (sceneName == '') {
        errorText = "You must specify a scene name with at least one character.";
    } else if (sceneName.length > 20) {
        errorText = "Your scene name cannot have more than 20 characters.";
    } else {
        for (var i = 0; i < data.filesystem.files.length; i++) {
            var name = data.filesystem.files[i].name;
            if (name.startsWith("/scene_") && sceneName == name.substring(7)) {
                errorText = 'A scene with name ' + sceneName + ' does already exist.';
                break;
            }
        }
    }

    if (errorText != '') {
        $('#scSceneContent').prepend('<div id="scSceneError" class="alert alert-dismissible alert-danger"><button type="button" class="close" data-dismiss="alert">&times;</button><p id="scSceneErrorText">' + errorText + '</p></div>');
    } else {
        sendRequestToLogo(domain + '/led/scene/add' + '?name=' + sceneName);
        $('#scSceneModalAddName').val('');
    }

}

/**
 * Removes an existing scene.
 */
function removeScene() {
    var scene = $('#scSceneSelect option:selected').text();
    sendRequestToLogo(domain + '/led/scene/remove' + '?name=' + scene);
}

/**
*  Assigns a new scene to a button.
*/
function changeButtonAssignment(button) {
    var scene = $('#scButtonSelector' + button + ' option:selected').text();
    sendRequestToLogo(domain + '/led/button/assign?button=' + button + '&scene=' + scene);
}


/**
 * Builds the JSON object to update the configuration.
 */
function buildConfigJSON() {

    for (var i = 0; i < data.config.length; i++) {
        if (data.config[i].name == 'DEFAULT_SPEED') {
            data.config[i].value = $('#confDefaultSpeed').val();
        } else if (data.config[i].name == 'MAX_DELAY') {
            data.config[i].value = $('#confMaxDelay').val();
        } else if (data.config[i].name == 'NEOPIXEL_NUMBER') {
            data.config[i].value = $('#confNeoPixelNumber').val();
        } else if (data.config[i].name == 'NEOPIXEL_PIN') {
            data.config[i].value = $('#confNeoPixelPin').val();
        } else if (data.config[i].name == 'WIFI_MDNS_NAME') {
            data.config[i].value = $('#confMDNSName').val();
        } else if (data.config[i].name == 'WIFI_NAME') {
            data.config[i].value = $('#confWiFiName').val();
        } else if (data.config[i].name == 'BUTTON_PINS') {
            data.config[i].value = $('#confButtonPins').val();
        }
    }

}

/**
 * Updates all configuration values from the config page.
 */
function updateConfiguration() {
    buildConfigJSON();
    sendRequestToLogo(domain + '/led/config/update' + '?json={"config":' + JSON.stringify(data.config) + '}');
}

/**
 * Updates all configuration values from the config page.
 */
function updateFirmware() {

    var url = domain + '/ota' + '?url=' + $('#ccFWUURL').val();
    if ($('#ccFWUURLFingerprint').val() && $('#ccFWUURLFingerprint').val() != "") {
        url += '&fingerprint=' + $('#ccFWUURLFingerprint').val();
    }
    sendRequestToLogo(url);
}

/**
 * Changes the startup scene for the logo.
 */
function changeStartScene() {
    sendRequestToLogo(domain + '/led/config/set' + '?key=START_SCENE&value=' + $('#sscSceneSelect').val());
}