import React from 'react';
import Element from './Element';

const MyComponent = ({ value }) => {
  return (
    <ul>
      {value.map((item, id) => (
        <Element key={id} value={item} />
      ))}
    </ul>
  );
};

export default MyComponent;