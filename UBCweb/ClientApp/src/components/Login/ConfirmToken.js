import PropTypes from 'prop-types';

export async function checkToken(TokenVal) {
    return fetch('http://192.168.43.66:5000/login/checkToken', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ TokenVal })
    }).then(data => data.json())
}

export default async function ConfirmToken({ token, setToken }) {
    if (token) {
        const confirmToken = await checkToken(token.TokenVal);
        if (confirmToken === "bad")
            setToken("");
    }
}

ConfirmToken.propTypes = {
    setToken: PropTypes.func.isRequired
};