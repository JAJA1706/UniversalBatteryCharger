import React, { useState } from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import Dashboard from "./components/Dashboard/Dashboard";
import Preferences from "./components/Preferences/Preferences";
import Login from "./components/Login/Login";
import Register from './components/Login//Register';
import useToken from "./components/Hooks/useToken";
import Header from "./components/Header/Header";
import ConfirmToken from "./components/Login/ConfirmToken";
import './App.css'

export default function App() {
    const [register, setRegister]= useState();
    const { token, setToken } = useToken();

    ConfirmToken({ token, setToken });
    if (!token) {
        if (register) {
            return (
                <div className="login-wrapper">
                    <Register />
                    <button type="login" onClick={() => setRegister(false)}>Przejdz do logowania</button>
                </div>
            );
        }
        else {
            return (
                <div className="login-wrapper">
                    <Login setToken={setToken} />
                    <button type="register" onClick={() => setRegister(true)}>Przejdz do rejestracji</button>
                </div>
            );
        }
    }

    return (
    <div>
        <Header TokenVal={token} setToken={setToken}/>
        <div className="wrapper">
            <BrowserRouter>
                <Switch>
                    <Route path="/dashboard">
                        <Dashboard />
                    </Route>
                    <Route path="/preferences">
                        <Preferences />
                    </Route>
                </Switch>
            </BrowserRouter>
        </div>
    </div>
    );
}