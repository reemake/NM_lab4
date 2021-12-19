#include <iostream>
#include <fstream>
using namespace std;

const double S = 1;

void getDataFromFile(ifstream& file, int& n, double& a, double& b, double& e, double& hMin) {
	if (!file.is_open())
	{
		cerr << "Невозможно открыть файл\n\n\n\n" << endl;
		system("pause");
		exit(1);
	}
	else {
		file >> n >> a >> b >> e >> hMin;
	}
}

void setDataToFile(ofstream& file, double I_h, double a, double b, double e, double Eps, double newN) {

	file << "IER = 0 - нет ошибки.\nРезультат:" << endl;
	file << "Значение вычисленного интеграла (I_h) = " << I_h << '\n' << "Точность полученного решения (Eps) = " << Eps << " <= заданного (e) = " << e << '\n' << "Шаг, при котором было получено решение (H) = " << (b - a) / newN << "\nОбщее число разбиений = " << newN << endl;
}

typedef double(*pointFunc)(double);

double integrableFunction(double x) {
	return (2 * x + 3 / sqrt(x));
}

double leftRectangleIntegral(pointFunc f, double a, double b, int n, float hMin) {
	double h = (b - a) / n;
	if (h < hMin) {
		throw 2.0;
	}
	double sum = 0.0;
	for (int i = 0; i < n; i++) {
		sum += h * f(a + i * h);
	}
	return sum;
}

int main() {
	
	setlocale(LC_ALL, "RUS");

	cout << "Лабораторная работа №4:\n\nВычисление определенного интеграла с апостериорной оценкой глобальной погрешности, максимальное допустимое значение которой задано.\n";
	cout << "Проверила: Шабунина З.А.\nПодготовил: Сафонов Н.С.\n\n\n";

	int n;
	double a, b, e, hMin;
	double I_1, I_2, H;
	ifstream fileInput("in.txt");
	ofstream fileOutput("out.txt");

	getDataFromFile(fileInput, n, a, b, e, hMin);
	cout << "n = " << n << endl;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "e = " << e << endl;
	cout << "hMin = " << hMin << endl;
	cout << endl;

	try {
		if (b <= a) {
			throw 3.0f;
		}

		I_1 = leftRectangleIntegral(integrableFunction, a, b, n, hMin);
		double Eps;
		int newN = n;
		do {
			newN *= 2;
			I_2 = leftRectangleIntegral(integrableFunction, a, b, newN, hMin);
			Eps = 1.0 * fabs((I_1 - I_2) / (pow(0.5, S) - 1));
			if (newN > 10e7) {
				throw 1;
			}
			if (Eps < e && n == newN / 2) {
				newN = n;
				break;
			}
			I_1 = I_2;
		} while (Eps > e);

		cout << "IER = 0 - нет ошибки." << endl;
		setDataToFile(fileOutput, I_1, a, b, e, Eps, newN);
	}
	catch (int) {
		fileOutput << "IER = 1 - решение не получено, т.к. погрешность перестала уменьшаться." << endl;
		cerr << "IER = 1 - решение не получено, т.к. погрешность перестала уменьшаться." << endl;
	}
	catch (double) {
		fileOutput << "IER = 2 - шаг интегрирования стал недопустимо мал." << endl;
		cerr << "IER = 2 - шаг интегрирования стал недопустимо мал." << endl;
	}
	catch (float) {
		fileOutput << "IER = 3 - ошибка входных данных, b <= a." << endl;
		cerr << "IER = 3 - ошибка входных данных, b <= a." << endl;
	}

	return 0;
}