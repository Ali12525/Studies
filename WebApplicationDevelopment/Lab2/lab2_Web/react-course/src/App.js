import React, { useState, useEffect, useMemo } from 'react';
import logo from './logo.svg';
import './App.css';
import MyComponent from './NewComponent';

function App() {

  const initialArray = [
    {
      id: 1,
      value: "Ali"
    },

    {
      id: 2,
      value: "Anton"
    },

    {
      id: 3,
      value: "Imran"
    },

    {
      id: 4,
      value: "Dan Kolbasenko"
    },

    {
      id: 5,
      value: "Max"
    },

    {
      id: 6,
      value: "Max"
    },

  ];

  const [searchTerm, setSearchTerm] = useState('');

  const filteredArray = useMemo(() => {
    if (!searchTerm) return initialArray.map(item => item.value);
    return initialArray.filter(item =>
      item.value.toLowerCase().includes(searchTerm.toLowerCase())
    );
  }, [searchTerm, initialArray]);

  useEffect(() => {

    if (searchTerm.trim() === '') {
      document.title = 'write for searchs';
    } else {
      document.title = `Found ${filteredArray.length} item(s)`;
    }
  }, [searchTerm, filteredArray]);

  useEffect(() => {
    document.title = 'useEffect hook example';
  }, [])

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        
        <input
          type="text"
          placeholder="Search..."
          value={searchTerm}
          onChange={(e) => setSearchTerm(e.target.value)}
          style={{ margin: '20px', padding: '8px', width: '300px' }}
        />

        {searchTerm.trim() !== '' && (
          <p>
            Found {filteredArray.length} result(s)
          </p>
        )}

        <MyComponent value={filteredArray} />

        <a
          className="App-link"
          href="https://reactjs.org"
          rel="noreferrer"
          target="_blank"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;