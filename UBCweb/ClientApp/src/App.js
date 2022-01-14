import React, { Component, useState } from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import Dashboard from "./components/Dashboard/Dashboard";
import Preferences from "./components/Preferences/Preferences";
import Login from "./components/Login/Login";
import Register from './components/Login//Register';
import useToken from "./components/Hooks/useToken";
import './App.css'

export default function App() {
    const [register, setRegister]= useState();
    const { token, setToken } = useToken();

    if (!token) {
        if (register) {
            return (
                <div className="button">
                    <Register />
                    <button type="login" onClick={() => setRegister(false)}>Login</button>
                </div>
            );
        }
        else {
            return (
                <div className="button">
                    <Login setToken={setToken} />
                    <button type="register" onClick={() => setRegister(true)}>Register</button>
                </div>
            );
        }
    }

    return (
    <div className="wrapper">
        <h1>Application</h1>
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
    );
}

  //render () {
  //  return (
  //    <Layout>
  //      <Route exact path='/' component={Home} />
  //      <Route path='/counter' component={Counter} />
  //      <Route path='/fetch-data' component={FetchData} />
  //    </Layout>
  //  );
  //}