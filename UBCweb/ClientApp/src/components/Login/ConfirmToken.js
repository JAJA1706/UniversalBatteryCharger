import PropTypes from 'prop-types';

export async function checkToken(TokenVal) {
    return fetch('http://10.1.9.237:5000/login/checkToken', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ TokenVal })
    }).then(data => data.json())
}

export default async function ConfirmToken({token, setToken}) {
    const confirmToken = await checkToken(token);
    if (confirmToken === "bad")
        setToken("");
}

ConfirmToken.propTypes = {
    setToken: PropTypes.func.isRequired
};