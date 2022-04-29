library("lmtest")
library("rlms")
library("dplyr")
library("car")


data <- rlms_read("r19i_os26c.sav")
data2 = select(data, oj13.2, o_age, oh5, o_educ, status, oj6.2, o_marst, oj24, oj72.51, oj1.1.1, oj6)
# oj72.51  - Во время обучения в ВУЗе Вы (работали / работаете)? 
# oj1.1.1 - Насколько Вы удовлетворены или не удовлетворены Вашей работой в целом? 
# oj6 - Есть ли у респондента подчиненные на этой работе?

# Удалим все строки со значениями NA
data2 = na.omit(data2)
glimpse(data2)

# Зарплата
sal = as.numeric(data2$mj13.2)
data2["salary"] = (sal - mean(sal)) / sqrt(var(sal))
data2["salary"]

# Возраст
age1 = as.numeric(data2$m_age)
data2["age"] = (age1 - mean(age1)) / sqrt(var(age1))
data2["age"]

# Пол
data2["sex"] = as.character(data2$mh5)
data2$sex[which(data2$sex != '1')] <- 0
data2$sex[which(data2$sex == '1')] <- 1
data2["sex"] = as.character(data2$sex)
data2["sex"]

# Наличие высшего образования
h_educ = as.character(data2$m_educ)
data2["higher_educ"] = data2$m_educ
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
dur1 = as.character(data2$mj6.2)
dur2 = lapply(dur1, as.integer)
dur3 = as.numeric(unlist(dur2))
data2["dur"] = (dur3 - mean(dur3)) / sqrt(var(dur3))
data2$dur

# Семейное положение
wed= as.character(data2$m_marst)
data2["wed1"]=data2$m_marst
data2$wed1 = 0
data2$wed1[which(wed == '1')] <- 1
data2$wed1[which(wed == '3')] <- 1
data2$wed1 = as.numeric(data2$wed1)

data2["wed2"]=data2$m_marst
data2$wed2 = 0
data2$wed2[which(wed == '2')] <- 1
data2$wed2 = as.numeric(data2$wed2)

data2["wed3"]=data2$m_marst
data2$wed3 = 0
data2$wed3[which(wed == '4')] <- 1
data2$wed3 = as.numeric(data2$wed3)

data2["wed4"]=data2$m_marst
data2$wed4 = 0
data2$wed4[which(wed=='5')] <- 1
data2$wed4 = as.numeric(data2$wed4)

# Работал ли респондент во время обучения в ВУЗе
data2["u_work"] = as.character(data2$sj72.51)
data2["uni_work"] = 1
data2$uni_work[which(data2$u_work == '4')] <- 0
data2["uni_work"] = as.numeric(data2$uni_work)
data2$uni_work

# Насколько респондент удовлетворен или не удовлетворен работой в целом? 
data2["sat"] = as.character(data2$sj1.1.1)
data2["work_sat"] = 0
data2$work_sat[which(data2$sat == '1')] <- 1
data2$work_sat[which(data2$sat == '2')] <- 1
data2$work_sat[which(data2$sat == '3')] <- 1
data2["work_sat"] = as.numeric(data2$work_sat)
data2$work_sat

# tсть ли у респондента подчиненные на этой работе
data2["sub"] = as.character(data2$oj6)
data2["subord"] = 0
data2$subord[which(data2$sub == '1')] <- 1
data2["subord"] = as.numeric(data2$subord)
data2$subord


data3 = select(data2, salary, age, sex, higher_educ, status1, dur, wed1, wed2, wed3, wed4, uni_work, work_sat, witted)


model1 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + wed2 + wed3 + wed4 + owner + second_job + language, data3)
summary(model1)
vif(model1)
# vif для параметров wed очень высокий, уберем wed2

model2 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + wed3 + wed4 + owner + second_job + language, data3)
summary(model2)
vif(model2)
# vif уменьшился, но у парметров wed3 и wed4 нет звездочек, также уберем их

model3 = lm(salary ~ age + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)
# vif каждого из параметров меньше 1.26, у каждого из них 3 звездочки, R^2 маленький - 18.4%


# Задание 2


# Рассмотрим модели с различными степенями переменных

model3 = lm(salary ~ I(age^0.1) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.2) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.3) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.4) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.5) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.6) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.7) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.8) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^0.9) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.1) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.2) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.3) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.4) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.5) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.6) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.7) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.8) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^1.9) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ I(age^2) + sex + higher_educ + status1 + dur + wed1 + owner + language, data3)
summary(model3)
vif(model3)



model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.1) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.2) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.3) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.4) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.5) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.6) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.7) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.8) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^0.9) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.1) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.2) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.3) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.4) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.5) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.6) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.7) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.8) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^1.9) + wed1 + owner + language, data3)
summary(model3)
vif(model3)

model3 = lm(salary ~ age + sex + higher_educ + status1 + I(dur^2) + wed1 + owner + language, data3)
summary(model3)
vif(model3)


# Задания 3, 4


model4 = lm(salary ~ I(age^1.2) + sex + higher_educ + status1 + I(dur^1.5) + wed1 + owner + language, data3)
summary(model4)
vif(model4)
# У этой модели самый высокий R^2 - 23.7%, vif всех переменных не больше 1.15, у каждой 3 звездочки

# У переменных age и wed1 отрицательный множитель - между ними и объясняемой переменной существует обратная взаимосвязь
# Чем моложе респондент, тем выше у него зарплата; если он не состоит в браке, то у него выше зарплата

# У переменных sex, higher_educ, status1, dur, owner, language положительный множитель -
# между ними и объясняемой переменной существует прямая взаимосвязь
# У мужчин; респондентов с высшим образованием; из города или областного центра; с большей продолжительностью рабочей недели
# совладельцев предприятия; владеющих иностранным языком - зарплата выше


# Задание 5


# Подмножество людей с высшим образованием
data4 = subset(data3, higher_educ == 1)

# Среди них выделим подмножество не из города
data5 = subset(data4, status1 == 0)

# Строим модель
model_subset1 = lm(salary ~ I(age^1.2) + sex + I(dur^1.5) + wed1 + owner + language, data5)
summary(model_subset1)
vif(model_subset1)
# R^2 = 10.7%, все переменные без звездочек, за исключением sex - одна звезда
# Модель плохая


# Среди людей с высшим образованием выделим подмножество женщин
data6 = subset(data4, sex == 0)

# Строим модель
model_subset2 = lm(salary ~ I(age^1.2) + status1 + I(dur^1.5) + wed1 + owner + language, data6)
summary(model_subset2)
vif(model_subset2)
# R^2 multiple = 6.2%, R^2 adjusted вовсе отрицательный, только перменные status1, dur и language имеют больше двух звезд
# остальные их не имеют, модель очень плохая
