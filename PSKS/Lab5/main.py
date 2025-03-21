# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("Skin_NonSkin.txt", sep="\\s+", header=None)
data.columns = ["B", "G", "R", "Label"]
print(data.head())
print(data.info())

data['Label'] = data['Label'] == 1
print(data.head())
print(data.info())

print("Дубликатов в данных:", data.duplicated().sum())
# Удаляем дубликаты
data = data.drop_duplicates()
print("Дубликатов в данных после удаления:", data.duplicated().sum())

print(data.describe(include='all'))

print(data['Label'].value_counts())

print(data[data['Label'] == True].describe())
print(data[data['Label'] == False].describe())

sample_data = data.sample(n=1000, random_state=42)

#Построим график одного из признаков
plt.figure() 
sample_data['R'].plot()  
plt.show()

#Представим признаки на графике в виде точек. 
plt.figure() 
ax = plt.gca() 
plt.grid(True) 
plt.title('Признаки') 
plt.plot(sample_data['B'], '.b')
plt.plot(sample_data['G'], '.g')
plt.plot(sample_data['R'], '.r')
plt.ylabel('Значение (B, G, R)') 
ax.legend(['B', 'G', 'R'])
plt.show()

#Диаграмма рассеяния, или точечная диаграмма:
from pandas.plotting import scatter_matrix
scatter_matrix(sample_data[['B','G', 'R']], alpha=0.5, figsize=(10, 10))

#Точечная диаграмма, на которой каждый класс объектов раскрашен в свой цвет.
from matplotlib.colors import ListedColormap 
cmap_bold = ListedColormap(['#FF0000', '#00FF00']) 
plt.figure() 
ax = plt.gca() 
plt.grid(True) 
plt.title('Scatter') 
plt.scatter(sample_data['B'], sample_data['G'], c=sample_data['Label'], cmap=cmap_bold)
plt.xlabel('B')  
plt.ylabel('G')  
plt.show()

#Диаграмма размаха
arr = sample_data[['B', 'G', 'R']].values
arr[np.isnan(arr)] = 0
plt.title('Boxplot для признаков B, G, R')
plt.boxplot(arr)
plt.show()

#Рассмотрим корреляцию между признаками
print(data.corr())