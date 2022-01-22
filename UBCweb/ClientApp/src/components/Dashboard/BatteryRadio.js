import React, { useEffect } from 'react';

export default function BatteryRadio({ selectedBattery, onRadioChange }) {

    useEffect(() => {
        const defualtRadio = document.getElementById("0");
        defualtRadio.defaultChecked = true;
    }, []);

    const radios = document.getElementsByClassName('radioBattery');
    for (let i = 0; i < radios.length; ++i) {
        if (radios[i].id === selectedBattery) {
            radios[i].defaultChecked = true;
        }
    }

    if (selectedBattery === 0) {

    } else {

    }

    return (
        <div onChange={onRadioChange}>
            <label>
                <input className="radioBattery" type="radio" name="options" value={0} id="0" />bateria1
            </label>
            <label>
                <input className="radioBattery" type="radio" name="options" value={1} id="1" />bateria2
            </label>
        </div>
    );
}