const firstInput = document.getElementById('firstValue');
const secondInput = document.getElementById('secondValue');
const btnCalc = document.getElementById('calcBtn');
const resultElem = document.getElementById('resultElem');

//Задание 1
function handleCalculate() {
    const firstValue = parseFloat(firstInput.value)
    const secondValue = parseFloat(secondInput.value)

    const result = Math.floor(firstValue * secondValue)
    console.log(`${firstValue} × ${secondValue} = ${result} `)
    resultElem.textContent = `${firstValue} × ${secondValue} = ${result}`
}
btnCalc.addEventListener('click', handleCalculate);

//Задание 2
const stringInput = document.getElementById('string');
const btnStr = document.getElementById('findBtn');
const resultString = document.getElementById('resultString');
function handleString() {
    let string = stringInput.value

    console.log(string.indexOf('incididunt'));
    resultString.textContent +=  String(string.indexOf('incididunt')) + ' ' + String(string.indexOf('dolore')) + ' ' + String(string.indexOf('exercitation')) + ' ' + String(string.indexOf('world')) +' ' + String(string.indexOf('tea'));
    console.log(string.indexOf('dolore'));
    console.log(string.indexOf('exercitation'));
    console.log(string.indexOf('world'));
    console.log(string.indexOf('tea'));
}
btnStr.addEventListener('click', handleString);

//Задание 3
const input = document.getElementById('value');
const arraySpan = document.getElementById('array');
const addBtn = document.getElementById('addBtn');
const sortBtn = document.getElementById('sortBtn');
const resultSpan = document.getElementById('resultArr');

let values = [];

function addValue() {
    const val = parseFloat(input.value);

    values.push(val);
    input.value = '';
    updateArrayDisplay();

    if (values.length === 4) {
        addBtn.disabled = true;
    }
}

function updateArrayDisplay() {
    arraySpan.textContent = `[${values.join(', ')}]`;
    console.log('[' + values.join(', ') + ']')
}

function quickSort(arr) {
    if (arr.length <= 1) return arr;
    const pivot = arr[0];
    const left = [];
    const right = [];
    for (let i = 1; i < arr.length; i++) {
        if (arr[i] < pivot) left.push(arr[i]);
        else right.push(arr[i]);
    }
    return [...quickSort(left), pivot, ...quickSort(right)];
}

function handleSort() {
    if (values.length === 0) {
        resultSpan.textContent = 'Массив пуст. Добавьте числа.';
        console.log('Массив пуст. Добавьте числа.')
        return;
    }

    const sorted = quickSort(values);
    resultSpan.textContent = `[${sorted.join(', ')}]`;
    console.log('[' + values.join(', ') + ']')

}

addBtn.addEventListener('click', addValue);
sortBtn.addEventListener('click', handleSort);

//Задание 4
const firstDel = document.getElementById('firstDel');
const secondDel = document.getElementById('secondDel');
const resultDel = document.getElementById('resultDel');
const delBtn = document.getElementById('delBtn');
function handleDel() {
    let firstValueDel = firstDel.value;
    let secondValueDel = secondDel.value;

    let a = Math.abs(firstValueDel);
    let b = Math.abs(secondValueDel);

    while (b !== 0) {
        let temp = b;
        b = a % b;
        a = temp;
    }

    const gcd = a;
    resultDel.textContent = (`НОД(${firstValueDel}, ${secondValueDel}) = ${gcd}`);
}
delBtn.addEventListener('click', handleDel);

//Задание 5
const textInput = document.getElementById('textInput');
const reverseBtn = document.getElementById('reverseBtn');
const reverseResult = document.getElementById('reverseResult');

function reverseStringRecursive(str) {
    if (str.length <= 1) {
        return str;
    }
    return reverseStringRecursive(str.substring(1)) + str[0];
}

function handleReverse() {
    const originalText = textInput.value;

    const reversedText = reverseStringRecursive(originalText);
    reverseResult.textContent = `Исходный текст: "${originalText}" → Перевёрнутый: "${reversedText}"`;
}

reverseBtn.addEventListener('click', handleReverse);

//Задание 6
const mergeBtn = document.getElementById('mergeBtn');
const mergeResult = document.getElementById('mergeResult');
const resultObj = document.getElementById('resultObj');
function mergeObjects() {
    // Три объекта, у каждого 2 ключа
    const obj1 = {
        name: 'Леха',
        age: 55
    };
    const obj2 = {
        city: 'Москва',
        profession: 'разработчик'
    };
    const obj3 = {
        hobby: 'фото',
        language: 'Русский'
    };

    const merged = [ obj1, obj2, obj3 ];

    console.log('Итоговый объект:', merged);
}
mergeBtn.addEventListener('click', mergeObjects);

//Задание 7
const arrResult = document.getElementById('arrResult');
const array = [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5];
const positiveNumbers = array.filter(num => num > 0);
arrResult.textContent = String(positiveNumbers);
console.log(positiveNumbers);

//Задание 8
const bgDiv = document.getElementById('bgColorDiv');
const colors = ['red', 'green', 'purple', 'black', 'white'];
let colorIndex = 0;

function changeColor() {
    bgDiv.style.backgroundColor = colors[colorIndex];
    colorIndex = (colorIndex + 1) % colors.length;
}
document.addEventListener('click', changeColor);