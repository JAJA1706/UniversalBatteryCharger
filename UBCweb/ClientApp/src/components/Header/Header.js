import React from 'react';
import PropTypes from 'prop-types';
import './Header.css'

async function logoutUser(TokenVal) {
    return fetch('http://10.1.9.237:5000/logout', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ TokenVal })
    })
}

export default function Header({ TokenVal, setToken }) {

    const handleLogout = () => {
        logoutUser(TokenVal);
        setToken("");
    }

    return (
        <div className="header">
            <div className="headerStyle">
                <h2 className="headerTextStyle"> Universal Battery Charger </h2>
                <button onClick={handleLogout} className="btn btn-dark">Wyloguj</button>
            </div>
        </div>
    );
}

Header.propTypes = {
    setToken: PropTypes.func.isRequired
};