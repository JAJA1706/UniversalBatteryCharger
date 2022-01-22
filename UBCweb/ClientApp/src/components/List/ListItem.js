import React, { Component } from 'react';

export default class ListItem extends Component {
    constructor(props) {
        super(props);
        this.onClick = this.onClick.bind(this);

    }

    onClick = () => {
        this.props.setSelectedRow(this.props.id);
    }

    render() {
        return (
            <div onClick={this.onClick} style={{ backgroundColor: this.props.color }}>
                <li>
                {this.props.value}
                </li>
            </div>
        );
    }
} 