# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler

from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LinearRegression, LogisticRegression
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier

from sklearn.ensemble import BaggingClassifier, AdaBoostClassifier, StackingClassifier

from sklearn.metrics import accuracy_score, precision_score, recall_score, classification_report

# Загрузка и предварительная обработка данных
data = pd.read_csv("Skin_NonSkin.txt", sep="\\s+", header=None)
data.columns = ["B", "G", "R", "Label"]

# Приводим метки к логическому типу (True для кожи)
data['Label'] = data['Label'] == 1

# Удаление дубликатов
data = data.drop_duplicates()

# Разделение на признаки и целевую переменную
X = data.loc[:, ['B', 'G', 'R']]
y = data.loc[:, 'Label']

# Разделение выборки на обучающую и тестовую (40% для теста)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.4, shuffle=True, random_state=42)

# Словарь для хранения результатов моделей
results = {}

###############################################
# 1. Метод ближайших соседей (KNN)
###############################################
print("Метод ближайших соседей:")
knn = KNeighborsClassifier()
# Параметр n_neighbors оптимизируется через GridSearchCV
param_dict = {'n_neighbors': list(range(1, 100))}
grid = GridSearchCV(knn, param_dict, scoring='accuracy', cv=5)
grid.fit(X_train, y_train)
best_n = grid.best_params_.get('n_neighbors')

knn = KNeighborsClassifier(n_neighbors=best_n)
knn.fit(X_train, y_train)
y_pred_knn = knn.predict(X_test)

acc_knn = accuracy_score(y_test, y_pred_knn)
prec_knn = precision_score(y_test, y_pred_knn, average='macro')
rec_knn = recall_score(y_test, y_pred_knn, average='macro')

results['KNN'] = {'accuracy': acc_knn, 'precision': prec_knn, 'recall': rec_knn}
print("Accuracy:", acc_knn)
print("Precision:", prec_knn)
print("Recall:", rec_knn)
print(classification_report(y_test, y_pred_knn, target_names=['NonSkin','Skin']))
print("\n--------------------------------------------\n")

###############################################
# 2. Метод наименьших квадратов (Least Squares)
###############################################
# Этот метод (на основе линейной регрессии) ищет линейную зависимость между признаками и 
# целевой переменной. Модель подбирает коэффициенты так, чтобы минимизировать сумму
# квадратов разности между предсказанными и истинными значениями.
print("Метод наименьших квадратов:")
ls_model = LinearRegression()
ls_model.fit(X_train, y_train)
# Предсказания (преобразуем в бинарный результат по порогу 0.5)
y_pred_ls = ls_model.predict(X_test)
y_pred_ls_binary = np.where(y_pred_ls >= 0.5, True, False)

acc_ls = accuracy_score(y_test, y_pred_ls_binary)
prec_ls = precision_score(y_test, y_pred_ls_binary, average='macro')
rec_ls = recall_score(y_test, y_pred_ls_binary, average='macro')

results['LeastSquares'] = {'accuracy': acc_ls, 'precision': prec_ls, 'recall': rec_ls}
print("Accuracy:", acc_ls)
print("Precision:", prec_ls)
print("Recall:", rec_ls)
print(classification_report(y_test, y_pred_ls_binary, target_names=['NonSkin','Skin']))
print("\n--------------------------------------------\n")

###############################################
# 3. Метод опорных векторов (SVM)
###############################################
# Метод опорных векторов ищет гиперплоскость, которая максимально разделяет классы в обучающем наборе.
#При использовании ядерной функции (в данном случае RBF — радиальная базисная функция) 
#SVM может работать с нелинейно разделимыми данными, преобразуя их в пространство более 
#высокой размерности, где данные становятся линейно разделимыми.
#Основная идея — найти такую гиперплоскость, расстояние (зазор) между которой и ближайшими 
#объектами каждого класса (опорными векторами) максимально.
print("Метод опорных векторов:")
svm_model = SVC(kernel='rbf', gamma='scale', C=1, random_state=42)
svm_model.fit(X_train, y_train)
y_pred_svm = svm_model.predict(X_test)

acc_svm = accuracy_score(y_test, y_pred_svm)
prec_svm = precision_score(y_test, y_pred_svm, average='macro')
rec_svm = recall_score(y_test, y_pred_svm, average='macro')

results['SVM'] = {'accuracy': acc_svm, 'precision': prec_svm, 'recall': rec_svm}
print("Accuracy:", acc_svm)
print("Precision:", prec_svm)
print("Recall:", rec_svm)
print(classification_report(y_test, y_pred_svm, target_names=['NonSkin','Skin']))
print("\n--------------------------------------------\n")

###############################################
# 4. Бустинг (Boosting)
###############################################
#Бустинг — это ансамблевый метод, в котором несколько слабых классификаторов (обычно простых моделей) 
#обучаются последовательно.
#Каждый новый классификатор пытается исправить ошибки предыдущих, 
#уделяя больше внимания тем наблюдениям, которые были классифицированы неправильно.
#Итоговое решение получается путём взвешенного объединения предсказаний всех слабых классификаторов.
print("Бустинг:")
boosting_model = AdaBoostClassifier(n_estimators=50, random_state=42)
boosting_model.fit(X_train, y_train)
y_pred_boost = boosting_model.predict(X_test)

acc_boost = accuracy_score(y_test, y_pred_boost)
prec_boost = precision_score(y_test, y_pred_boost, average='macro')
rec_boost = recall_score(y_test, y_pred_boost, average='macro')

results['Boosting'] = {'accuracy': acc_boost, 'precision': prec_boost, 'recall': rec_boost}
print("Accuracy:", acc_boost)
print("Precision:", prec_boost)
print("Recall:", rec_boost)
print(classification_report(y_test, y_pred_boost, target_names=['NonSkin','Skin']))
print("\n--------------------------------------------\n")

###############################################
# 5. Стеккинг (Stacking)
###############################################
#Стеккинг объединяет несколько различных моделей (базовых классификаторов), обученных 
#на одних и тех же данных, и использует их предсказания в качестве признаков для метамодели.
#Базовые модели (например, KNN, дерево решений, SVM) дают свои 
#предсказания, которые затем подаются на вход финальной модели (в данном случае LogisticRegression).
#Такая комбинация озволяет использовать сильные стороны каждого отдельного метода, 
#что зачастую приводит к улучшению качества предсказания.
print("Стеккинг:")
# Базовые классификаторы
estimators = [
    ('knn', KNeighborsClassifier(n_neighbors=best_n)),
    ('dt', DecisionTreeClassifier()),
    ('svm', SVC(kernel='rbf', gamma='scale', C=1, probability=True, random_state=42))
]
stacking_model = StackingClassifier(estimators=estimators, final_estimator=LogisticRegression(), cv=5)
stacking_model.fit(X_train, y_train)
y_pred_stack = stacking_model.predict(X_test)

acc_stack = accuracy_score(y_test, y_pred_stack)
prec_stack = precision_score(y_test, y_pred_stack, average='macro')
rec_stack = recall_score(y_test, y_pred_stack, average='macro')

results['Stacking'] = {'accuracy': acc_stack, 'precision': prec_stack, 'recall': rec_stack}
print("Accuracy:", acc_stack)
print("Precision:", prec_stack)
print("Recall:", rec_stack)
print(classification_report(y_test, y_pred_stack, target_names=['NonSkin','Skin']))
print("\n--------------------------------------------\n")

###############################################
# Итоговый вывод: какая модель лучше по accuracy
###############################################
print("Сводная таблица результатов:")
for method, metrics in results.items():
    print(f"{method}: Accuracy = {metrics['accuracy']:.4f}, Precision = {metrics['precision']:.4f}, Recall = {metrics['recall']:.4f}")

best_method = max(results, key=lambda x: results[x]['accuracy'])
print("\nЛучший метод по accuracy:", best_method)
