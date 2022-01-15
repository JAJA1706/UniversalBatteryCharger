import { useState } from 'react';

export default function useToken() {

    const getTokenFromStorage = () => {
        const tokenString = localStorage.getItem('Token');
        const userToken = JSON.parse(tokenString);
        return userToken?.TokenVal;
    };

    const [token, setToken] = useState(getTokenFromStorage());

    const saveToken = userToken => {
        localStorage.setItem('Token', JSON.stringify(userToken));
        setToken(userToken.TokenVal);
    };

    return {
        token,
        setToken: saveToken
    };

}