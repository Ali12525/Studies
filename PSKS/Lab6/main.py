# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

data = pd.read_csv("Skin_NonSkin.txt", sep="\\s+", header=None)
data.columns = ["B", "G", "R", "Label"]

# Переводим в булевый вид
data['Label'] = data['Label'] == 1

# Удаляем дубликаты
print("Дубликатов в данных:", data.duplicated().sum())
data = data.drop_duplicates()
print("Дубликатов в данных после удаления:", data.duplicated().sum())

# Находим выбросы
Q25, Q75 = np.percentile(data["B"], 25), np.percentile(data["BD"], 75)
IQR = Q75 - Q25
print("IQR:", IQR)
k_factor = IQR * 1.5
print("k_factor:", k_factor)
low_value, high_value = Q25 - k_factor, Q75 + k_factor
print("limits:", low_value, high_value)
flier_ind = [k for x, k in zip(data["R"], range(0, data.shape[0] - 1)) if x < low_value or x > high_value]
print("Выбросов: ", len(flier_ind))

# Находим выбросы
Q25, Q75 = np.percentile(data["R"], 25), np.percentile(data["R"], 75)
IQR = Q75 - Q25
print("IQR:", IQR)
k_factor = IQR * 1.5
print("k_factor:", k_factor)
low_value, high_value = Q25 - k_factor, Q75 + k_factor
print("limits:", low_value, high_value)
flier_ind = [k for x, k in zip(data["R"], range(0, data.shape[0] - 1)) if x < low_value or x > high_value]
print("Выбросов: ", len(flier_ind))

# Нормализация
scaler = StandardScaler()
scaled_values = scaler.fit_transform(data[['B', 'G', 'R']].values.astype(float))
data[['B', 'G', 'R']] = scaled_values

# Сохраняем таблицу в csv
data.to_csv('data_clean_norm.csv', index= False)
print(data.head(5))

data.loc[:,['B','G','R']].hist() 
plt.show()