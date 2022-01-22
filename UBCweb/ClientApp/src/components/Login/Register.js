import React, { useState } from 'react';

async function RegisterUser(credentials) {
    return fetch('http://192.168.43.66:5000/register', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(credentials)
    })
        .then(data => data.json())
}

export default function Register() {
    const [username, setUserName] = useState();
    const [password, setPassword] = useState();
    const [password2, setPassword2] = useState();
    const [resultText, setResultText] = useState();
    const [resultColor, setResultColor] = useState({ color: 'red' });

    const handleSubmit = async e => {
        e.preventDefault();
        const result = await RegisterUser({
            username,
            password,
            password2
        });
        if (result === "profil utworzony")
            setResultColor({ color: "blue" });
        else
            setResultColor({ color: "red" });
        setResultText(result);
    }

    return (
        <div>
            <h1>Zarejestruj sie</h1>
            <form>
                <label>
                    <p>Nazwa Uzytkownika</p>
                    <input type="text" onChange={e => setUserName(e.target.value)} />
                </label>
                <label>
                    <p>Haslo</p>
                    <input type="password" onChange={e => setPassword(e.target.value)} />
                </label>
                <label>
                    <p>Powtorz haslo</p>
                    <input type="password" onChange={e => setPassword2(e.target.value)} />
                </label>
                <div>
                    <p style={resultColor}>{resultText}</p>
                    <button type="submit" onClick={handleSubmit}>Zatwierdz</button>
                </div>
            </form>
        </div>
    );
}