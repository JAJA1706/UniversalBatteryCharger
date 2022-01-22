import React, { useState, useEffect } from 'react';

async function getBatteryMonitorData(userID, canalID, battery, profileNum) {
    const data = await fetch('http://192.168.43.66:5000/UserData/GetMonitorData/' + userID + '/' + canalID + '/' + battery)
    const jsonData = await data.json();
    return jsonData;
}

export default function Monitor({ userID, canalID, battery }) {
    const [completionPercent, setCompletionPercent] = useState('');
    const [cellVoltage, setCellVoltage] = useState('');
    const [currentFlowing, setCurrentFlowing] = useState('');

    useEffect(() => {
        const getServerData = async () => {
            const data = await getBatteryMonitorData(userID, canalID, battery);
            setCompletionPercent(data.Completion);
            setCellVoltage(data.CellVoltage);
            setCurrentFlowing(data.CurrentFlowing);
        }

        getServerData();
    }, [userID, canalID, battery]);

    return (
        <div>
            <table className="table">
                <thead className="thead-dark">
                    <tr>
                        <th scope="col">Procent Naladowania</th>
                        <th scope="col">Napiecie Baterii</th>
                        <th scope="col">Plynacy Prad</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>{completionPercent}%</td>
                        <td>{cellVoltage}mV</td>
                        <td>{currentFlowing}mA</td>
                    </tr>
                </tbody>
            </table>
        </div>
    );
}