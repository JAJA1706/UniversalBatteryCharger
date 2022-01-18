import React, { Component } from 'react';
import ListItem from '../List/ListItem';
import BatteryForm from './BatteryForm';
import "./Dashboard.css"

export default class Dashboard extends Component{
    constructor(props) {
        super(props);
        this.state = {
            selectedCanal: -1,
            loadingData: true,
            canalData: [],
            canalDataMessage: ""
        }
        this.setSelectedCanal = this.setSelectedCanal.bind(this);
        this.getCanalList = this.getCanalList.bind(this);
        this.addCanal = this.addCanal.bind(this);
        this.removeCanal = this.removeCanal.bind(this);
    }

    componentDidMount() {
        this.getCanalList();
    }

    setSelectedCanal(x) {
        this.setState({selectedCanal: x})
    }

    render() {
        let mainList = [];
        if (this.state.loadingData) {
            mainList[0] = "ladowanie"
        }
        else {
            const props = this.state.canalData;
            for (let i = 0; i < props.length; i++) {
                if (i === this.state.selectedCanal)
                    mainList[i] = <ListItem key={i} id={i} value={"kanal " + props[i].canalNum} setSelectedRow={this.setSelectedCanal} color={'green'} />
                else
                    mainList[i] = <ListItem key={i} id={i} value={"kanal " + props[i].canalNum} setSelectedRow={this.setSelectedCanal} color={'white'} />
            }
        }

        if (this.state.selectedCanal === -1) {
            return (
                <div className="canalStyle">
                    <ul>{mainList}</ul>
                    <p>{this.state.canalDataMessage}</p>
                    <div>
                        <button type="button" onClick={this.addCanal}>Dodaj nowy kanal</button>
                        <button type="button" onClick={this.removeCanal}>Usun zaznaczony</button>
                    </div>
                </div>
            );
        }
        else {
            return (
                <div>
                    <div className="canalStyle">
                        <ul>{mainList}</ul>
                        <p>{this.state.canalDataMessage}</p>
                        <div>
                            <button type="button" onClick={this.addCanal}>Dodaj nowy kanal</button>
                            <button type="button" onClick={this.removeCanal}>Usun zaznaczony</button>
                        </div>
                    </div>
                    <hr />
                    <BatteryForm userID={this.props.userID} canalID={this.state.selectedCanal} />
                </div>
            );
        }
    }

    async getCanalList() {
        const data = await fetch('http://localhost:5000/UserData/GetCanals/' + this.props.userID);
        const jsonData = await data.json();
        this.setState({ canalData: jsonData, loadingData: false });
    }

    async addCanal() {
        const response = await fetch('http://localhost:5000/UserData/AddCanal/' + this.props.userID, {
            method: 'PUT'
        }).then(data => data.json())

        if (response === "ok")
            this.getCanalList();
        else
            this.setState({ canalDataMessage: response });
    }

    async removeCanal() {
        const canalNum = this.state.selectedCanal;
        await fetch('http://localhost:5000/UserData/RemoveCanal/' + this.props.userID, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ canalNum } )
        }).then(data => data.json())

        await this.setState({ selectedCanal: -1, canalDataMessage: "" });
        this.getCanalList();
    }
}