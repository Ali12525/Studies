#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import scipy.special
import imageio
import matplotlib.pyplot as plt
import os
from PIL import Image  # для изменения размера изображения (если необходимо)

# Определение класса нейронной сети
class NeuralNetwork:
    def __init__(self, inputnodes, hiddennodes, outputnodes, learningrate):
        self.inodes = inputnodes
        self.hnodes = hiddennodes
        self.onodes = outputnodes

        # Инициализация весов: нормальное распределение со средним 0 и стандартным отклонением,
        # равным обратному квадратному корню от количества входных (или скрытых) узлов.
        self.wih = np.random.normal(0.0, pow(self.inodes, -0.5), (self.hnodes, self.inodes))
        self.who = np.random.normal(0.0, pow(self.hnodes, -0.5), (self.onodes, self.hnodes))

        self.lr = learningrate
        # Функция активации – сигмоида (expit)
        self.activation_function = lambda x: scipy.special.expit(x)
    
    def train(self, inputs_list, targets_list):
        # Передаем входные данные в виде столбца (вектор-столбец)
        inputs = np.array(inputs_list, ndmin=2).T
        targets = np.array(targets_list, ndmin=2).T

        # Вычисляем входные данные скрытого слоя
        hidden_inputs = np.dot(self.wih, inputs)
        hidden_outputs = self.activation_function(hidden_inputs)

        # Вычисляем входные данные итогового слоя
        final_inputs = np.dot(self.who, hidden_outputs)
        final_outputs = self.activation_function(final_inputs)

        # Вычисляем ошибки итогового слоя (таргеты - полученный ответ)
        output_errors = targets - final_outputs
        # Ошибка скрытого слоя – передается обратно через веса
        hidden_errors = np.dot(self.who.T, output_errors)

        # Корректировка весов между скрытым и итоговым слоями
        self.who += self.lr * np.dot((output_errors * final_outputs * (1.0 - final_outputs)),
                                      hidden_outputs.T)
        # Корректировка весов между входным и скрытым слоями
        self.wih += self.lr * np.dot((hidden_errors * hidden_outputs * (1.0 - hidden_outputs)),
                                      inputs.T)
    
    def query(self, inputs_list):
        inputs = np.array(inputs_list, ndmin=2).T

        # Вычисляем входные данные скрытого слоя
        hidden_inputs = np.dot(self.wih, inputs)
        hidden_outputs = self.activation_function(hidden_inputs)

        # Вычисляем входные данные итогового слоя
        final_inputs = np.dot(self.who, hidden_outputs)
        final_outputs = self.activation_function(final_inputs)

        return final_outputs

# Основные параметры сети
input_nodes = 784      # 28x28 пикселей
hidden_nodes = 160   # число скрытых узлов (можно изменять)

output_nodes = 10      # цифры от 0 до 9
learning_rate = 0.1

# Создаем экземпляр нейронной сети
n = NeuralNetwork(input_nodes, hidden_nodes, output_nodes, learning_rate)

# Загружаем обучающий набор данных из файла mnist_train.csv
with open("mnist_train.csv", 'r') as f:
    training_data_list = f.readlines()

# Обучаем нейросеть (обучение проходит 5 эпох)
epochs = 6
for e in range(epochs):
    print("Эпоха:", e + 1)
    for record in training_data_list:
        all_values = record.strip().split(',')
        # Первый элемент – метка (правильная цифра)
        targets = np.zeros(output_nodes) + 0.01
        targets[int(all_values[0])] = 0.99
        # Нормируем входные данные (значения пикселей от 0 до 255 преобразуем в диапазон 0.01 – 1.0)
        inputs = (np.asfarray(all_values[1:]) / 255.0 * 0.99) + 0.01
        n.train(inputs, targets)

# Тестирование нейросети на наборе данных из файла mnist_test.csv
with open("mnist_test.csv", 'r') as f:
    test_data_list = f.readlines()

# Сохраняем результаты тестирования (правильный ответ или ошибка)
scorecard = []
for record in test_data_list:
    all_values = record.strip().split(',')
    correct_label = int(all_values[0])
    inputs = (np.asfarray(all_values[1:]) / 255.0 * 0.99) + 0.01
    outputs = n.query(inputs)
    label = np.argmax(outputs)
    if label == correct_label:
        scorecard.append(1)
    else:
        scorecard.append(0)

scorecard_array = np.asarray(scorecard)
accuracy = scorecard_array.sum() / scorecard_array.size * 100
print("Эффективность сети на тестовых данных: {:.2f}%".format(accuracy))

# Распознавание собственного изображения (my1.png)
img_filename = "my2.png"
# Загружаем изображение в градациях серого
img_array = imageio.v2.imread(img_filename, mode='F')

# Если изображение не 28x28, то меняем его размер с помощью PIL
if img_array.shape != (28, 28):
    img = Image.fromarray(np.uint8(img_array))
    img = img.resize((28, 28))
    img_array = np.array(img)   

# Преобразование изображения: инвертируем цвета (MNIST: белый фон, чёрная цифра)
img_data = 255.0 - img_array.reshape(784)
# Нормируем данные в диапазоне [0.01, 1.0]
img_data = (img_data / 255.0 * 0.99) + 0.01

# Выводим изображение для визуальной проверки
plt.figure(figsize=(3, 3))
plt.imshow(img_array, cmap='gray')
plt.title("Распознаваемое изображение")
plt.show()

# Запрашиваем сеть для распознавания цифры
outputs = n.query(img_data)
predicted_label = np.argmax(outputs)
print("Нейросеть распознала цифру:", predicted_label)
