
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
                hues: 6,
                saturations: 1,
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
 * Render the SCENE card.
 * -------------------------------------------------------------------------------------------------------------------------------------------
 */
function renderSceneCard() {
    $('#scSceneSelect').empty();

    $('#scSceneButtonRemove').hide();
    $('#scSceneButtonEnable').hide();

    $('#scSceneSelect').append('<option selected="">Select a scene...</option>');

    for (var i = 0; i < data.filesystem.files.length; i++) {
        var name = data.filesystem.files[i].name;
        if (name.startsWith("/scene_")) {
            name = name.substring(7);
            $('#scSceneSelect').append('<option value="' + name + '">' + name + '</option>');
        }
    }
}

/**
 * Updates the page from the JSON data structure send by the logo.
 */
function updatePage() {

    renderModeCard();
    renderColorCard();
    renderSpeedCard();
    renderSceneCard();



}
