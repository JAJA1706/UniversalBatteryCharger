import React, { Component } from 'react';
import ListItem from '../List/ListItem';
import BatteryRadio from './BatteryRadio';
import ProfileForm from './ProfileForm';
import Monitor from './Monitor';
import "./BatteryForm.css"

export default class BatteryForm extends Component {
    constructor(props) {
        super(props);
        this.state = {
            currentCanal: -1,
            batteryChosen: 0,
            batteryMode: "nie rob nic",
            capacity: 0,
            minVoltage: 0,
            selectedProfile: -1,
            profilesData: [],
            loadingData: true,
            profileDataMessage: ""
        }
        this.setSelectedProfile = this.setSelectedProfile.bind(this);
        this.saveChanges = this.saveChanges.bind(this);
        this.onRadioChange = this.onRadioChange.bind(this);
        this.saveBatteryData = this.saveBatteryData.bind(this);
        this.getServerData = this.getServerData.bind(this);
        this.getBatteryData = this.getBatteryData.bind(this);
        this.getProfileList = this.getProfileList.bind(this);
        this.addProfile = this.addProfile.bind(this);
    }

    componentDidMount(){
        this.getServerData();
    }
    componentDidUpdate(prevProps, prevState) {
        if (prevProps.canalID !== this.props.canalID || prevState.selectedProfile !== this.state.selectedProfile || prevState.batteryChosen !== this.state.batteryChosen) {
            this.getServerData();
        }
    }

    setSelectedProfile(x) {
        this.setState({ selectedProfile: x })
    }

    async onRadioChange(event) {
        await this.setState({ batteryChosen: event.target.value });
        this.getBatteryData();
    }

    async saveChanges() {
        const data = { BatteryMode: this.state.batteryMode, Capacity: this.state.capacity, MinVoltage: this.state.minVoltage }
        await this.saveBatteryData(data);
    }

    async getServerData() {
        this.getBatteryData();
        await this.getProfileList();
        if (this.state.profilesData.length < 4) {
            this.setState({ profileDataMessage: "" });
        }
        if (this.state.batteryMode === "") {
            this.setState({ batteryMode: "nie rob nic" });
        }
    }

    render() {

        let mainList = [];
        if (this.state.loadingData) {
            mainList[0] = "ladowanie"
        }
        else {
            const props = this.state.profilesData;
            for (let i = 0; i < props.length; i++) {
                mainList[i] = <ListItem key={i} id={i} value={"profil " + props[i].profileNum} setSelectedRow={this.setSelectedProfile} color={'white'} />
            }
        }

        if (this.state.selectedProfile === -1) {
            return (
                <div className="batteryAndMonitor">
                    <div className="batteryFormStyle">
                        <BatteryRadio selectedBattery={this.state.batteryChosen} onRadioChange={this.onRadioChange} />
                        <form>
                            <label>
                                <p>Pojemnosc</p>
                                <input type="text" onChange={e => this.setState({ capacity: e.target.value })} value={this.state.capacity} />
                            </label>
                            <label>
                                <p>Minimalne napiecie</p>
                                <input type="text" onChange={e => this.setState({ minVoltage: e.target.value })} value={this.state.minVoltage} />
                            </label>
                            <div>
                                <select className="form-control" onChange={e => this.setState({ batteryMode: e.target.value })} value={this.state.batteryMode} >
                                    <option>nie rob nic</option>
                                    <option>laduj</option>
                                    <option>rozladuj</option>
                                </select>
                            </div>
                            <div>
                                <ul>{mainList}</ul>
                                <p>{this.state.profileDataMessage}</p>
                                <button type="button" onClick={this.saveChanges}>Zapisz zmiany</button>
                                <button type="button" onClick={this.addProfile}>Dodaj Profil</button>
                            </div>
                        </form>
                    </div>
                    <div>
                        <Monitor userID={this.props.userID} canalID={this.props.canalID} battery={this.state.batteryChosen} />
                    </div>
                </div>
            );
        }
        else {
            return (
                <ProfileForm userID={this.props.userID} canalID={this.props.canalID} battery={this.state.batteryChosen} profile={this.state.selectedProfile} setSelectedProfile={this.setSelectedProfile} />
            );
        }
    }

    async saveBatteryData(data) {
        await fetch('http://192.168.43.66:5000/UserData/SaveBattery/' + this.props.userID + '/' + this.props.canalID + '/' + this.state.batteryChosen, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
            },
            body: JSON.stringify( data )
        });
    }

    async getBatteryData() {
        const data = await fetch('http://192.168.43.66:5000/UserData/GetBattery/' + this.props.userID + '/' + this.props.canalID + '/' + this.state.batteryChosen)
        const jsonData = await data.json();
        this.setState({
            batteryMode: jsonData.BatteryMode,
            capacity: jsonData.Capacity,
            minVoltage: jsonData.MinVoltage
        })
    }

    async getProfileList() {
        const data = await fetch('http://192.168.43.66:5000/UserData/GetProfilesList/' + this.props.userID + '/' + this.props.canalID + '/' + this.state.batteryChosen);
        const jsonData = await data.json();
        this.setState({ profilesData: jsonData, loadingData: false });
    }

    async addProfile() {
        const response = await fetch('http://192.168.43.66:5000/UserData/AddProfile/' + this.props.userID + '/' + this.props.canalID + '/' + this.state.batteryChosen, {
            method: 'PUT'
        }).then(data => data.json())

        if (response === "ok")
            this.getProfileList();
        else
            this.setState({ profileDataMessage: response });
    }
}