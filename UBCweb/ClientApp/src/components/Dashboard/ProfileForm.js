import React, { useState, useEffect } from 'react';
import './ProfileForm.css';

async function saveProfile(profile, userID, canalID, battery, profileNum) {
    return fetch('http://localhost:5000/UserData/SaveProfile/' + userID + '/' + canalID + '/' + battery + '/' + profileNum, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(profile)
    })
}

async function removeProfile(userID, canalID, battery, profileNum) {
    await fetch('http://localhost:5000/UserData/RemoveProfile/' + userID + '/' + canalID + '/' + battery + '/' + profileNum, {
        method: 'POST'
    });
}

async function getProfile(userID, canalID, battery, profileNum) {
    const data = await fetch('http://localhost:5000/UserData/GetProfile/' + userID + '/' + canalID + '/' + battery + '/' + profileNum)
    const jsonData = await data.json();
    return jsonData;
}

export default function ProfileForm({ userID, canalID, battery, profile, setSelectedProfile }) {
    const [componentMounted, setComponentMounted] = useState(false);
    const [method, setMethod] = useState('staly prad');
    const [maxVoltage, setMaxVoltage] = useState('');
    const [maxCurrent, setMaxCurrent] = useState('');
    const [maxTemperature, setMaxTemperature] = useState('');
    const [maxTime, setMaxTime] = useState('');
    const [desiredVoltage, setDesiredVoltage] = useState('');
    const [desiredCurrent, setDesiredCurrent] = useState('');
    const [voltageDeltaInverval, setVoltageDeltaInverval] = useState('');
    const [minVoltageDelta, setMinVoltageDelta] = useState('');
    const [temperatureDeltaInterval, setTemperatureDeltaInterval] = useState('');
    const [maxTemperatureDelta, setMaxTemperatureDelta] = useState('');
    const [endingVoltageDrop, setEndingVoltageDrop] = useState('');
    const [lookForEndingVoltageDrop, setLookForEndingVoltageDrop] = useState('tak');

    const returnToBatteryForm = () => {
        setSelectedProfile(-1);
    }

    const handleSubmit = async e => {
        await saveProfile({
            method,
            maxVoltage,
            maxCurrent,
            maxTemperature,
            maxTime,
            desiredVoltage,
            desiredCurrent,
            voltageDeltaInverval,
            minVoltageDelta,
            temperatureDeltaInterval,
            maxTemperatureDelta,
            endingVoltageDrop,
            lookForEndingVoltageDrop
        }, userID, canalID, battery, profile);
    }

    const handleRemove = async e => {
        await removeProfile(userID, canalID, battery, profile);
        returnToBatteryForm();
    }

    useEffect(() => {
        const getServerData = async () => {
            const data = await getProfile(userID, canalID, battery, profile);
            if (data.method === "") {
                setMethod("staly prad");
            } else {
                setMethod(data.method);
            }
            setMaxVoltage(data.maxVoltage);
            setMaxCurrent(data.maxCurrent);
            setMaxTemperature(data.maxTemperature);
            setMaxTime(data.maxTime);
            setDesiredVoltage(data.desiredVoltage);
            setDesiredCurrent(data.desiredCurrent);
            setVoltageDeltaInverval(data.voltageDeltaInverval);
            setMinVoltageDelta(data.minVoltageDelta);
            setTemperatureDeltaInterval(data.temperatureDeltaInterval);
            setMaxTemperatureDelta(data.maxTemperatureDelta);
            setEndingVoltageDrop(data.endingVoltageDrop);
            if (data.lookForEndingVoltageDrop === "") {
                setLookForEndingVoltageDrop("tak");
            } else {
                setLookForEndingVoltageDrop(data.method);
            }
        }

        const returnToBatteryForm = () => {
            setSelectedProfile(-1);
        }

        if (componentMounted) {
            returnToBatteryForm();
        }
        else {
            getServerData();
        }
        setComponentMounted(true);
    }, [canalID, battery, profile, userID, setSelectedProfile]);

    return (
        <div>
            <h1>profil {profile}</h1>
            <button type="button" onClick={returnToBatteryForm}>Wroc</button>
            <button type="button" onClick={handleRemove}>Usun profil</button>
            <form className="formStyle">
                <label>
                    <p>Metoda</p>
                    <select className="form-control" onChange={e => setMethod(e.target.value)} defaultValue={method} >
                        <option>staly prad</option>
                        <option>stale napiecie</option>
                        <option>prad podtrzymania</option>
                    </select>
                </label>
                <label>
                    <p>Maks Napiecie</p>
                    <input type="text" onChange={e => setMaxVoltage(e.target.value)} value={maxVoltage} />
                </label>
                <label>
                    <p>Maks Prad</p>
                    <input type="text" onChange={e => setMaxCurrent(e.target.value)} value={maxCurrent} />
                </label>
                <label>
                    <p>Maks Temperatura</p>
                    <input type="text" onChange={e => setMaxTemperature(e.target.value)} value={maxTemperature} />
                </label>
                <label>
                    <p>Maks Czas</p>
                    <input type="text" onChange={e => setMaxTime(e.target.value)} value={maxTime} />
                </label>
                <label>
                    <p>Zamierzony Napiecie</p>
                    <input type="text" onChange={e => setDesiredVoltage(e.target.value)} value={desiredVoltage} />
                </label>
                <label>
                    <p>Zamierzony Prad</p>
                    <input type="text" onChange={e => setDesiredCurrent(e.target.value)} value={desiredCurrent} />
                </label>
                <label>
                    <p>Czas probkowania napiecia</p>
                    <input type="text" onChange={e => setVoltageDeltaInverval(e.target.value)} value={voltageDeltaInverval} />
                </label>
                <label>
                    <p>Minimalna roznica sprobkowanych napiec</p>
                    <input type="text" onChange={e => setMinVoltageDelta(e.target.value)} value={minVoltageDelta} />
                </label>
                <label>
                    <p>Czas probkowania temperatury</p>
                    <input type="text" onChange={e => setTemperatureDeltaInterval(e.target.value)} value={temperatureDeltaInterval} />
                </label>
                <label>
                    <p>Maksymalna roznica sprobkowanych temperatur</p>
                    <input type="text" onChange={e => setMaxTemperatureDelta(e.target.value)} value={maxTemperatureDelta} />
                </label>
                <label>
                    <p>Koncowy spadek napiecia</p>
                    <input type="text" onChange={e => setEndingVoltageDrop(e.target.value)} value={endingVoltageDrop} />
                </label>
                <label>
                    <p>Czy zwracac uwage na koncowy spadek napiecia</p>
                    <select className="form-control" onChange={e => setLookForEndingVoltageDrop(e.target.value)} value={lookForEndingVoltageDrop} >
                        <option>tak</option>
                        <option>nie</option>
                    </select>
                </label>
                <div>
                    <button type="submit" onClick={handleSubmit}>Zatwierdz</button>
                </div>
            </form>
        </div>
    );
}