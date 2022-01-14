import React, { useState } from 'react';
import PropTypes from 'prop-types';

async function loginUser(credentials) {
    return fetch('http://10.1.9.237:5000/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(credentials)
    })
        .then(data => data.json())
}

export default function Login({ setToken }) {
    const [username, setUserName] = useState();
    const [password, setPassword] = useState();
    const [resultText, setResultText] = useState();

    const handleSubmit = async e => {
        e.preventDefault();
        const token = await loginUser({
            username,
            password
        });
        if (token === "no")
            setResultText("uzytkownik o takiej kombinacji nazwy i hasla nie istnieje");
        else
            setToken(token);
    }

    return (
        <div>
            <h1>Zaloguj sie</h1>
            <form>
                <label>
                    <p>Uzytkownik</p>
                    <input type="text" onChange={e => setUserName(e.target.value)}/>
                </label>
                <label>
                    <p>Haslo</p>
                    <input type="password" onChange={e => setPassword(e.target.value)} />
                </label>
                <div>
                    <p style={{ color: 'red' }}>{resultText}</p>
                    <button type="submit" onClick={handleSubmit}>Zatwierdz</button>
                </div>
            </form>
        </div>
    )
}

Login.propTypes = {
    setToken: PropTypes.func.isRequired
};