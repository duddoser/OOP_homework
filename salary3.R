library("lmtest")
library("rlms")
library("dplyr")
library("car")


data <- rlms_read("r19i_os26c.sav")
data2 = select(data, oj13.2, o_age, oh5, o_educ, status, oj6.2, o_marst, oj24, oj260 , oj1.1.1, oj6)
# oj260  - Владеет ли респондент иностранным языком, помимо языков бывших стран СНГ? 
# oj1.1.1 - Насколько Вы удовлетворены или не удовлетворены Вашей работой в целом? 
# oj6 - Есть ли у респондента подчиненные на этой работе?

# Удалим все строки со значениями NA
data2 = na.omit(data2)

# Зарплата
sal = as.numeric(data2$oj13.2)
data2["salary"] = (sal - mean(sal)) / sqrt(var(sal))
data2["salary"]

# Возраст
age1 = as.numeric(data2$o_age)
data2["age"] = (age1 - mean(age1)) / sqrt(var(age1))
data2["age"]

# Пол
data2["sex"] = as.character(data2$oh5)
data2$sex[which(data2$sex != '1')] <- 0
data2$sex[which(data2$sex == '1')] <- 1
data2["sex"] = as.character(data2$sex)
data2["sex"]

# Наличие высшего образования
h_educ = as.character(data2$o_educ)
data2["higher_educ"] = data2$o_educ
data2["higher_educ"] = 0
data2$higher_educ[which(h_educ == '21')] <- 1
data2$higher_educ[which(h_educ == '22')] <- 1
data2$higher_educ[which(h_educ == '23')] <- 1
data2["higher_educ"] = as.numeric(data2$higher_educ)
data2["higher_educ"]

# Населенный пункт
status2 = as.character(data2$status)
data2["status1"] = 0
data2$status1[which(status2 == '1')] <- 1
data2$status1[which(status2 == '2')] <- 1
data2["status1"] = as.numeric(data2$status1)
data2$status1

# Длительность рабочей недели
dur1 = as.character(data2$oj6.2)
dur2 = lapply(dur1, as.integer)
dur3 = as.numeric(unlist(dur2))
data2["dur"] = (dur3 - mean(dur3)) / sqrt(var(dur3))
data2$dur

# Семейное положение
wed = as.character(data2$o_marst)
data2["wed1"]=data2$o_marst
data2$wed1 = 0
data2$wed1[which(wed == '1')] <- 1
data2$wed1[which(wed == '3')] <- 1
data2$wed1 = as.numeric(data2$wed1)

data2["wed2"]=data2$o_marst
data2$wed2 = 0
data2$wed2[which(wed == '2')] <- 1
data2$wed2 = as.numeric(data2$wed2)

data2["wed3"]=data2$o_marst
data2$wed3 = 0
data2$wed3[which(wed == '4')] <- 1
data2$wed3 = as.numeric(data2$wed3)

data2["wed4"]=data2$o_marst
data2$wed4 = 0
data2$wed4[which(wed=='5')] <- 1
data2$wed4 = as.numeric(data2$wed4)

data2["wed5"]=data2$o_marst
data2$wed5 = 0
data2$wed5[which(wed=='1')] <- 1
data2$wed5 = as.numeric(data2$wed5)

# Владение иностранным языком
data2["lan"] = as.character(data2$oj260)
data2["language"] = 0
data2$language[which(data2$lan == '1')] <- 1
data2["language"] = as.numeric(data2$language)
data2$language

# Насколько респондент удовлетворен или не удовлетворен работой в целом
data2["sat"] = as.character(data2$oj1.1.1)
data2["work_sat"] = 1
data2$work_sat[which(data2$sat == '4')] <- 0
data2["work_sat"] = as.numeric(data2$work_sat)
data2$work_sat

# есть ли у респондента подчиненные на этой работе
data2["sub"] = as.character(data2$oj6)
data2["subord"] = 0
data2$subord[which(data2$sub == '1')] <- 1
data2["subord"] = as.numeric(data2$subord)
data2$subord

data3 = select(data2, salary, age, sex, higher_educ, status1, dur, wed1, wed2, wed3, wed4, wed5, language, work_sat, subord)

model1 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + wed2 + wed3 + wed4 + language + work_sat + subord, data3)
summary(model1)
vif(model1)
# значение vif для параметра wed2 = 31 (высокий), попробуем убрать его

model2 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + wed3 + wed4 + language + work_sat + subord, data3)
summary(model2)
vif(model2)
# значение R^2 не изменилось, т.е. модель не испортилась, а значение vif уменьшилось (<2 у всех переменных)
# у параметров wed3 и wed4 нет звездочек, уберем их (хоть значение vif у параметров маленькое)

model3 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + language + work_sat + subord, data3)
summary(model3)
vif(model3)
# уберем параметр wed1, поскольку у него одна звездочка

model4 = lm(salary ~ age + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)
# значение R^2 исходной модели практически не отличается от полученной, т.е. модель хорошая, R^2 = 0.18
# значение vif всех параметров <2, и все параметры имеют 3 звездочки


model4 = lm(salary ~I(age^0.1) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.2) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.3) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.4) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.5) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.6) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.7) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.8) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^0.9) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.1) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.2) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.4) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.5) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.6) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.7) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.8) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.9) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^2) + sex + higher_educ + status1 + dur + language + work_sat + subord, data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + log(subord), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + log(status1) + dur + language + work_sat + log(subord), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.1), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.2), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.3), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.4), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + log(language) + work_sat + I(subord^0.5), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.6), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.7), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + log(sex) + higher_educ + status1 + dur + language + work_sat + I(subord^0.8), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^0.9), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + log(dur) + language + work_sat + I(subord^1.1), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + I(dur^0.5) + language + work_sat + I(subord^1.2), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + I(dur^2) + language + work_sat + I(subord^1.3), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + I(dur^4) + language + work_sat + I(subord^1.4), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^1.5), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^1.6), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + I(work_sat^2) + I(subord^1.7), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^1.8), data3) # top
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + log(work_sat) + I(subord^1.9), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^2), data3)
summary(model4)
vif(model4)

model4 = lm(salary ~I(age^1.3) + sex + higher_educ + status1 + dur + language + work_sat + I(subord^1.8), data3) # top
summary(model4)
vif(model4)
# У этой модели самый высокий R^2 = 0.20, значение vif всех переменных <2, у каждой 3 звездочки

# У пераметра age отрицательный множитель, т.е. между ним и объясняемой переменной существует обратная взаимосвязь
# Чем моложе респондент, тем выше у него зарплата

# У переменных sex, higher_educ, status1, dur, language, work_sat, subord положительный множитель, следовательно
# между ними и объясняемой переменной существует прямая взаимосвязь
# У мужчин, респондентов с высшим образованием, респондентов из города или областного центра,
# респондентов с большей продолжительностью рабочей недели, владеющих иностранным языком респондентов,
# удовлетворенных работой респондентов, имеющих подчиненных респондентов - зарплата выше

# респонденты, не состоявшие в браке
data4 = subset(data3, wed5 == 1)

# Среди них выделим подмножество городских жителей
data5 = subset(data4, status1 == 1)

# Строим модель
model_set1 = lm(salary ~ I(age^1.3) + sex + higher_educ + dur + language + work_sat + I(subord^1.8), data5)
summary(model_set1)
vif(model_set1)
# R^2 = 0.02, все переменные без звездочек - модель плохая


# женщины
data7 = subset(data3, sex == 0)
# разведенные женщины
data8 = subset(data7, wed3 == 1)
#  разведенные женщины без высшего образования
data9 = subset(data8, higher_educ == 0)

# Строим модель
model_set2 = lm(salary ~ I(age^1.3) + dur  + status1 + language + work_sat + I(subord^1.8), data9)
summary(model_set2)
vif(model_set2)
# значение R^2 = 0.13, переменные work_sat и subord имеют две звезды
# значение vif параметров < 1.042538, модель в целом плоха
