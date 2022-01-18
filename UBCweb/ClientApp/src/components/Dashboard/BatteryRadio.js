import React from 'react';

export default function BatteryRadio({ onRadioChange }) {
    return (
        <div onChange={onRadioChange}>
            <label>
                <input type="radio" name="options" value={0} defaultChecked />bateria1
            </label>
            <label>
                <input type="radio" name="options" value={1} />bateria2
            </label>
        </div>
    );
}