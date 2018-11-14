
/**
 * Render the DROPDOWN for the MODE card
 * -------------------------------------------------------------------------------------------------------------------------------------------
 */
function renderModeCard() {
    $('#scModeData').html(data.mode);
    $('#scModeDropDown').empty()

    if (data.mode === 'Off') {
        $('#scModeDropDown').append('<button class="dropdown-item active" onclick="sendRequestToLogo(\'' + domain + '/led/off\');">Off</button>');
    } else {
        $('#scModeDropDown').append('<button class="dropdown-item" onclick="sendRequestToLogo(\'' + domain + '/led/off\');">Off</button>');
    }

    $('#scModeDropDown').append('<div class="dropdown-divider"></div>');
    for (var i = 0; i < data.modes.length; i++) {

        if (data.modes[i].name !== 'Off') {
            var element = '<button class=\"dropdown-item';
            if (data.modes[i].name === data.mode) {
                element += ' active';
            }
            element += '" onclick="sendRequestToLogo(\'' + domain + data.modes[i].path + '\');">' + data.modes[i].name + '</button>';
            $('#scModeDropDown').append(element);
        }

    }
}

/**
 * Render the color selector for the COLOR card
 * -------------------------------------------------------------------------------------------------------------------------------------------
 */
function renderColorCard() {
    $('#scColorCtColors').empty();

    if (data.usage == 'Strip') {

        $('#scColorCtSingleUsage').hide()
        $('#scColorCtNoneUsage').hide()
        $('#scColorCtColors').show();

        $('#scColorButtonAll').show();
        $('#scColorButtonUpdate').show();

        $('#scColorButtonUpdate').css('width', '49%');

        for (var i = 0; i < data.leds; i++) {
            var element = '<div id="statusColorDiv' + i + '" style="display: inline-block;"><svg width="48" height="48" viewBox="0 0 48 48" class="ledDisplay">';
            element += '<circle id="statusColor' + i + '" style="opacity:1;fill:#' + data.colors[i] + ';" cx="24" cy="24" r="24" />';
            element += '</svg></div>';

            $("#scColorCtColors").append(element);

            var hueb = new Huebee('#statusColorDiv' + i, {
                setText: false,
                setBGColor: false,
                notation: 'hex',
                hues: 8,
                saturations: 3,
                shades: 4,
                className: 'dark-picker'
            });

            hueb.on('change', function (color, hue, sat, lum) {
                var id = this.anchor.id.substring(14);
                data.colors[id] = color.substring(1, color.length);
                lastColor = color;
                $('#statusColor' + id).css('fill', color);
                this.close();
            })

        }
    } else if (data.usage == 'None') {
        $('#scColorCtSingleUsage').hide()
        $('#scColorCtNoneUsage').show();
        $('#scColorCtColors').hide();

        $('#scColorButtonAll').hide();
        $('#scColorButtonUpdate').hide();

    } else if (data.usage == 'Single') {
        $('#scColorCtSingleUsage').show()
        $('#scColorCtNoneUsage').hide();
        $('#scColorCtColors').hide();

        $('#scColorButtonAll').hide();
        $('#scColorButtonUpdate').show();

        $('#scColorButtonUpdate').css('width', '100%');
        $('#scColorDataSingleColor').css('fill', data.colors[0]);
    }
}

/**
* Render the speed data for the SPEED card
* -------------------------------------------------------------------------------------------------------------------------------------------
*/
function renderSpeedCard() {

    if (data.mode == 'Static' || data.mode == 'Off') {
        $('#scSpeedCtDataSpeed').hide();
        $('#scSpeedCtDataNoSpeed').show();
        $('#scSpeedButton').hide();

    } else {
        $('#scSpeedCtDataSpeed').show();
        $('#scSpeedCtDataNoSpeed').hide();
        $('#scSpeedButton').show();

        $('#scSpeedDataProgressSpeed').attr('aria-valuenow', '' + data.speed).css('width', data.speed + '%');
    }
}

/**
 * Render the PROGRAMABLE SCENE card.
 * -------------------------------------------------------------------------------------------------------------------------------------------
 */
function renderProgramableSceneCard() {

}

/**
 * Loads the available scenes and displays it on the scenes tab.
 */
function renderSceneCards() {

    $('#sscSceneContent').empty();

    $('#sscSceneContent').append('<button type="button" class="btn-info list-group-item list-group-item-action" onclick="loadScene(\'Off\')">Off</button>');

    for (var i = 0; i < data.scenes.length; i++) {
        $('#sscSceneContent').append('<button type="button" class="bg-primary list-group-item list-group-item-action" onclick="loadScene(\'' + data.scenes[i] + '\')">' + data.scenes[i] + '</button>');
    }


    $('#scSceneSelect').empty();

    $('#scSceneButtonRemove').hide();
    $('#scSceneButtonEnable').hide();

    $('#scSceneSelect').append('<option selected="">Select a scene...</option>');

    for (var i = 0; i < data.scenes.length; i++) {
        $('#scSceneSelect').append('<option value="' + data.scenes[i] + '">' + data.scenes[i] + '</option>');
    }

    $('#scButtonsContent').empty();

    for (var i = 0; i < data.buttons.length; i++) {
        var element = '<li class="list-group-item flex-column align-items-start">';
        element += '<div class="d-flex w-100 justify-content-between">';
        element += '<h5 class="pt-2 mr-3">Button ' + data.buttons[i].name + '</h5>';
        element += '<select id="scButtonSelector' + data.buttons[i].name + '" class="custom-select bg-secondary" style="color: #ffffff; width:60%;" onchange="changeButtonAssignment(\'' + data.buttons[i].name + '\')">';

        if (data.buttons[i].scene == 'Off') {
            element += '<option value="Off" selected="">Off</option>';
        } else {
            element += '<option value="Off">Off</option>';
        }


        for (var j = 0; j < data.scenes.length; j++) {
            if (data.buttons[i].scene == data.scenes[j]) {
                element += '<option value="' + data.scenes[j] + '" selected="">' + data.scenes[j] + '</option>';
            } else {
                element += '<option value="' + data.scenes[j] + '">' + data.scenes[j] + '</option>';
            }
        }

        element += '</select></div></li>';
        $('#scButtonsContent').append(element);
    }




}


/**
 * Updates the page from the JSON data structure send by the logo.
 */
function updatePage() {

    renderSceneCards();
    renderModeCard();
    renderColorCard();
    renderSpeedCard();
    renderProgramableSceneCard();




}
