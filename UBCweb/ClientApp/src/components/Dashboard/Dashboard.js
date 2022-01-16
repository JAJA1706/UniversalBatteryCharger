import React, { Component } from 'react';
import ListItem from '../List/ListItem';

export default class Dashboard extends Component{
    constructor(props) {
        super(props);
        this.state = {
            selectedRow: -1,
            loadingData: true,
            canalData: []
        }
        this.setSelectedRow = this.setSelectedRow.bind(this);
        this.getCanalList = this.getCanalList.bind(this);
    }

    componentDidMount() {
        this.getCanalList(5);
    }

    async getCanalList(userID) {
        const data = await fetch('http://localhost:5000/UserData/GetCanals/' + userID);
        const jsonData = await data.json();
        console.log(jsonData);
        this.setState({ canalData: jsonData, loadingData: false });
    }

    setSelectedRow = (x) => {
        this.setState({selectedRow: x})
    }

    render() {
        let mainList = [];
        if (this.state.loadingData) {
            mainList[0] = "ladowanie"
        }
        else {
            const props = this.state.canalData;
            for (let i = 0; i < props.length; i++) {
                if (i === this.state.selectedRow)
                    mainList[i] = <ListItem key={i} id={i} value={props[i].Name} setSelectedRow={this.setSelectedRow} color={'red'} />
                else
                    mainList[i] = <ListItem key={i} id={i} value={props[i].Name} setSelectedRow={this.setSelectedRow} color={'white'} />
            }
        }

        return (
            <div>
                <h2>Dashboard</h2>
                <ul>{mainList}</ul>
                <div>
                    <button>Dodaj nowy kanal</button>
                    <button>Usun zaznaczony</button>
                </div>
            </div>
        );
    }
}