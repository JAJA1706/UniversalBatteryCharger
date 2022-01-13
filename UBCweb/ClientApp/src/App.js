import React, { Component, useState } from 'react';
import { Layout } from './components/Layout';
import { Home } from './components/Home';
import { FetchData } from './components/FetchData';
import { Counter } from './components/Counter';

import './App.css'
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import Dashboard from "./components/Dashboard/Dashboard";
import Preferences from "./components/Preferences/Preferences";
import Login from "./components/Login/Login";

export default class App extends Component {
    static displayName = App.name;

    constructor(props) {
        super(props);
        this.state = {
            token: false,
            number: 10
        };
        this.setToken = this.setToken.bind(this);
    }

    setToken() {
        this.setState({ token: true });
    }

    render() {
        //const [token, setToken] = useState();
        //if (!token) {
        //    return <Login setToken={setToken} />
        //}
        if (!this.state.token) {
            return <Login setToken={this.setToken} />
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
}
