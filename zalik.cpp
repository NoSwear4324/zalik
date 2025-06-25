#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::remove_if and std::find_if
#include <limits>    // For std::numeric_limits

// --- Structures ---

// Ward
struct Ward {
    int number; // Ward number
    std::vector<int> doctor_ids; // IDs of doctors assigned to the ward

    // Default constructor for vector initialization
    Ward() : number(0) {}
    Ward(int num) : number(num) {}
};

// Department
struct Department {
    int id; // Unique ID for department
    std::string name; // Department name
    std::vector<Ward> wards; // List of wards

    // Default constructor for vector initialization
    Department() : id(0) {}
    Department(int department_id, const std::string& dept_name) : id(department_id), name(dept_name) {}
};

// Doctor
struct Doctor {
    int id;
    std::string surname;
    std::string name;
    std::string patronymic;
    int birth_year;
    std::string position;
    std::string phone;
    std::vector<int> ward_numbers; // Ward numbers assigned to the doctor

    // Default constructor for vector initialization
    Doctor() : id(0), birth_year(0) {}
    Doctor(int doc_id, const std::string& sname, const std::string& fname, const std::string& pname, int b_year, const std::string& pos, const std::string& ph)
        : id(doc_id), surname(sname), name(fname), patronymic(pname), birth_year(b_year), position(pos), phone(ph) {}
};

// Analysis
struct Analysis {
    std::string name;
    std::string result;
    std::string date;

    // Default constructor for vector initialization
    Analysis() {}
    Analysis(const std::string& an_name, const std::string& an_result, const std::string& an_date)
        : name(an_name), result(an_result), date(an_date) {}
};

// Patient
struct Patient {
    int id; // Unique ID for patient
    std::string surname;
    std::string name;
    std::string patronymic;
    int birth_year;
    std::string primary_diagnosis;
    std::string admission_date;
    int department_id;
    int ward_number;
    int doctor_id;
    std::vector<Analysis> analyses;
    std::string discharge_date;

    // Default constructor for vector initialization
    Patient() : id(0), birth_year(0), department_id(0), ward_number(0), doctor_id(0) {}
    Patient(int pat_id, const std::string& sname, const std::string& fname, const std::string& pname, int b_year, const std::string& diag, const std::string& adm_date, int dept_id, int ward_num, int doc_id)
        : id(pat_id), surname(sname), name(fname), patronymic(pname), birth_year(b_year), primary_diagnosis(diag), admission_date(adm_date), department_id(dept_id), ward_number(ward_num), doctor_id(doc_id) {}
};

// --- Global Data Storage ---
std::vector<Department> departments;
std::vector<Doctor> doctors;
std::vector<Patient> patients;

int next_department_id = 1;
int next_doctor_id = 1;
int next_patient_id = 1;

// --- Helper Functions ---

void clear_input_buffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Department Management ---

void add_department() {
    Department new_dept;
    new_dept.id = next_department_id++;
    std::cout << "Введіть назву відділення: ";
    std::cin.ignore(); // Consume the newline character left by previous input
    std::getline(std::cin, new_dept.name);

    int num_wards;
    std::cout << "Введіть кількість палат у відділенні " << new_dept.name << ": ";
    while (!(std::cin >> num_wards) || num_wards <= 0) {
        std::cout << "Некоректний ввід. Будь ласка, введіть додатнє ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    clear_input_buffer(); // Consume newline after num_wards

    for (int i = 0; i < num_wards; ++i) {
        Ward new_ward;
        std::cout << "Введіть номер палати " << i + 1 << " для відділення " << new_dept.name << ": ";
        while (!(std::cin >> new_ward.number) || new_ward.number <= 0) {
            std::cout << "Некоректний ввід. Будь ласка, введіть додатнє ціле число: ";
            std::cin.clear();
            clear_input_buffer();
        }
        clear_input_buffer(); // Consume newline after new_ward.number
        new_dept.wards.push_back(new_ward);
    }
    departments.push_back(new_dept);
    std::cout << "Відділення '" << new_dept.name << "' додано з ID: " << new_dept.id << std::endl;
}

void display_departments() {
    if (departments.empty()) {
        std::cout << "Немає зареєстрованих відділень." << std::endl;
        return;
    }
    std::cout << "\n--- Список Відділень ---" << std::endl;
    for (const auto& dept : departments) {
        std::cout << "ID: " << dept.id << ", Назва: " << dept.name << std::endl;
        if (!dept.wards.empty()) {
            std::cout << "  Палати: ";
            for (const auto& ward : dept.wards) {
                std::cout << ward.number << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "------------------------" << std::endl;
}

Department* find_department_by_id(int id) {
    for (auto& dept : departments) {
        if (dept.id == id) {
            return &dept;
        }
    }
    return nullptr;
}

Ward* find_ward_in_department(Department* dept, int ward_num) {
    if (!dept) return nullptr;
    for (auto& ward : dept->wards) {
        if (ward.number == ward_num) {
            return &ward;
        }
    }
    return nullptr;
}

// --- Doctor Management ---

void add_doctor() {
    Doctor new_doc;
    new_doc.id = next_doctor_id++;
    std::cout << "Введіть прізвище лікаря: ";
    std::cin.ignore();
    std::getline(std::cin, new_doc.surname);
    std::cout << "Введіть ім'я лікаря: ";
    std::getline(std::cin, new_doc.name);
    std::cout << "Введіть по батькові лікаря: ";
    std::getline(std::cin, new_doc.patronymic);
    std::cout << "Введіть рік народження лікаря: ";
    while (!(std::cin >> new_doc.birth_year) || new_doc.birth_year <= 1900 || new_doc.birth_year > 2025) {
        std::cout << "Некоректний ввід. Будь ласка, введіть рік (наприклад, 1980): ";
        std::cin.clear();
        clear_input_buffer();
    }
    std::cin.ignore();
    std::cout << "Введіть посаду лікаря: ";
    std::getline(std::cin, new_doc.position);
    std::cout << "Введіть телефон лікаря: ";
    std::getline(std::cin, new_doc.phone);

    doctors.push_back(new_doc);
    std::cout << "Лікар '" << new_doc.surname << " " << new_doc.name << "' доданий з ID: " << new_doc.id << std::endl;
}

void display_doctors() {
    if (doctors.empty()) {
        std::cout << "Немає зареєстрованих лікарів." << std::endl;
        return;
    }
    std::cout << "\n--- Список Лікарів ---" << std::endl;
    for (const auto& doc : doctors) {
        std::cout << "ID: " << doc.id << ", ПІБ: " << doc.surname << " " << doc.name << " " << doc.patronymic
                  << ", Посада: " << doc.position << ", Тел: " << doc.phone << std::endl;
        if (!doc.ward_numbers.empty()) {
            std::cout << "  Закріплені палати: ";
            for (int ward_num : doc.ward_numbers) {
                std::cout << ward_num << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "----------------------" << std::endl;
}

Doctor* find_doctor_by_id(int id) {
    for (auto& doc : doctors) {
        if (doc.id == id) {
            return &doc;
        }
    }
    return nullptr;
}

void assign_doctor_to_ward() {
    if (doctors.empty() || departments.empty()) {
        std::cout << "Будь ласка, спочатку додайте лікарів та відділення." << std::endl;
        return;
    }

    display_doctors();
    int doc_id;
    std::cout << "Введіть ID лікаря, якого потрібно закріпити за палатою: ";
    while (!(std::cin >> doc_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Doctor* doc = find_doctor_by_id(doc_id);
    if (!doc) {
        std::cout << "Лікаря з таким ID не знайдено." << std::endl;
        return;
    }

    display_departments();
    int dept_id;
    std::cout << "Введіть ID відділення, до якого належить палата: ";
    while (!(std::cin >> dept_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Department* dept = find_department_by_id(dept_id);
    if (!dept) {
        std::cout << "Відділення з таким ID не знайдено." << std::endl;
        return;
    }

    int ward_num;
    std::cout << "Введіть номер палати, за якою потрібно закріпити лікаря (з відділення " << dept->name << "): ";
    while (!(std::cin >> ward_num)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Ward* ward = find_ward_in_department(dept, ward_num);
    if (!ward) {
        std::cout << "Палату " << ward_num << " не знайдено у відділенні " << dept->name << "." << std::endl;
        return;
    }

    // Check if doctor is already assigned to this ward
    bool already_assigned = false;
    for (int d_id : ward->doctor_ids) {
        if (d_id == doc->id) {
            already_assigned = true;
            break;
        }
    }

    if (already_assigned) {
        std::cout << "Лікар вже закріплений за цією палатою." << std::endl;
    } else {
        ward->doctor_ids.push_back(doc->id);
        doc->ward_numbers.push_back(ward->number);
        std::cout << "Лікар " << doc->surname << " закріплений за палатою " << ward->number << " у відділенні " << dept->name << "." << std::endl;
    }
}


// --- Patient Management ---

void add_patient() {
    if (departments.empty() || doctors.empty()) {
        std::cout << "Будь ласка, спочатку додайте відділення та лікарів." << std::endl;
        return;
    }

    Patient new_pat;
    new_pat.id = next_patient_id++;
    std::cout << "Введіть прізвище пацієнта: ";
    std::cin.ignore();
    std::getline(std::cin, new_pat.surname);
    std::cout << "Введіть ім'я пацієнта: ";
    std::getline(std::cin, new_pat.name);
    std::cout << "Введіть по батькові пацієнта: ";
    std::getline(std::cin, new_pat.patronymic);
    std::cout << "Введіть рік народження пацієнта: ";
    while (!(std::cin >> new_pat.birth_year) || new_pat.birth_year <= 1900 || new_pat.birth_year > 2025) {
        std::cout << "Некоректний ввід. Будь ласка, введіть рік (наприклад, 1990): ";
        std::cin.clear();
        clear_input_buffer();
    }
    std::cin.ignore();
    std::cout << "Введіть основний діагноз: ";
    std::getline(std::cin, new_pat.primary_diagnosis);
    std::cout << "Введіть дату госпіталізації (РРРР-ММ-ДД): ";
    std::getline(std::cin, new_pat.admission_date);

    // Assign to Department
    display_departments();
    std::cout << "Введіть ID відділення, куди госпіталізується пацієнт: ";
    while (!(std::cin >> new_pat.department_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Department* assigned_dept = find_department_by_id(new_pat.department_id);
    if (!assigned_dept) {
        std::cout << "Відділення з таким ID не знайдено. Пацієнт не доданий." << std::endl;
        return;
    }
    std::cin.ignore(); // Consume newline after department_id

    // Assign to Ward within Department
    std::cout << "Доступні палати у відділенні " << assigned_dept->name << ": ";
    for (const auto& ward : assigned_dept->wards) {
        std::cout << ward.number << " ";
    }
    std::cout << "\nВведіть номер палати, куди госпіталізується пацієнт: ";
    while (!(std::cin >> new_pat.ward_number)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Ward* assigned_ward = find_ward_in_department(assigned_dept, new_pat.ward_number);
    if (!assigned_ward) {
        std::cout << "Палату " << new_pat.ward_number << " не знайдено у відділенні " << assigned_dept->name << ". Пацієнт не доданий." << std::endl;
        return;
    }
    std::cin.ignore(); // Consume newline after ward_number

    // Assign to Doctor
    display_doctors();
    std::cout << "Введіть ID лікуючого лікаря: ";
    while (!(std::cin >> new_pat.doctor_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Doctor* assigned_doctor = find_doctor_by_id(new_pat.doctor_id);
    if (!assigned_doctor) {
        std::cout << "Лікаря з таким ID не знайдено. Пацієнт не доданий." << std::endl;
        return;
    }
    std::cin.ignore(); // Consume newline after doctor_id

    patients.push_back(new_pat);
    std::cout << "Пацієнт '" << new_pat.surname << " " << new_pat.name << "' доданий з ID: " << new_pat.id << std::endl;
}

void display_patients() {
    if (patients.empty()) {
        std::cout << "Немає зареєстрованих пацієнтів." << std::endl;
        return;
    }
    std::cout << "\n--- Список Пацієнтів ---" << std::endl;
    for (const auto& pat : patients) {
        std::cout << "ID: " << pat.id << ", ПІБ: " << pat.surname << " " << pat.name << " " << pat.patronymic
                  << ", Діагноз: " << pat.primary_diagnosis << ", Дата госп.: " << pat.admission_date
                  << ", Відділення ID: " << pat.department_id << ", Палата: " << pat.ward_number
                  << ", Лікар ID: " << pat.doctor_id;
        if (!pat.discharge_date.empty()) {
            std::cout << ", Дата виписки: " << pat.discharge_date;
        }
        std::cout << std::endl;

        if (!pat.analyses.empty()) {
            std::cout << "  Аналізи:" << std::endl;
            for (const auto& an : pat.analyses) {
                std::cout << "    - " << an.name << ": " << an.result << " (" << an.date << ")" << std::endl;
            }
        }
    }
    std::cout << "------------------------" << std::endl;
}

Patient* find_patient_by_id(int id) {
    for (auto& pat : patients) {
        if (pat.id == id) {
            return &pat;
        }
    }
    return nullptr;
}

void add_analysis_to_patient() {
    if (patients.empty()) {
        std::cout << "Немає зареєстрованих пацієнтів, щоб додати аналізи." << std::endl;
        return;
    }

    display_patients();
    int pat_id;
    std::cout << "Введіть ID пацієнта, якому потрібно додати аналіз: ";
    while (!(std::cin >> pat_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Patient* pat = find_patient_by_id(pat_id);
    if (!pat) {
        std::cout << "Пацієнта з таким ID не знайдено." << std::endl;
        return;
    }

    Analysis new_analysis;
    std::cin.ignore();
    std::cout << "Введіть назву аналізу: ";
    std::getline(std::cin, new_analysis.name);
    std::cout << "Введіть результат аналізу: ";
    std::getline(std::cin, new_analysis.result);
    std::cout << "Введіть дату аналізу (РРРР-ММ-ДД): ";
    std::getline(std::cin, new_analysis.date);

    pat->analyses.push_back(new_analysis);
    std::cout << "Аналіз додано пацієнту " << pat->surname << " " << pat->name << "." << std::endl;
}

void discharge_patient() {
    if (patients.empty()) {
        std::cout << "Немає зареєстрованих пацієнтів." << std::endl;
        return;
    }

    display_patients();
    int pat_id;
    std::cout << "Введіть ID пацієнта для виписки: ";
    while (!(std::cin >> pat_id)) {
        std::cout << "Некоректний ввід. Будь ласка, введіть ціле число: ";
        std::cin.clear();
        clear_input_buffer();
    }
    Patient* pat = find_patient_by_id(pat_id);
    if (!pat) {
        std::cout << "Пацієнта з таким ID не знайдено." << std::endl;
        return;
    }

    if (!pat->discharge_date.empty()) {
        std::cout << "Пацієнт вже виписаний " << pat->discharge_date << std::endl;
        return;
    }

    std::cin.ignore();
    std::cout << "Введіть дату виписки (РРРР-ММ-ДД): ";
    std::getline(std::cin, pat->discharge_date);

    std::cout << "Пацієнт " << pat->surname << " " << pat->name << " виписаний " << pat->discharge_date << "." << std::endl;
}

// --- Main Menu ---

void display_menu() {
    std::cout << "\n--- Система Управління Лікарнею ---" << std::endl;
    std::cout << "1. Додати відділення" << std::endl;
    std::cout << "2. Показати відділення" << std::endl;
    std::cout << "3. Додати лікаря" << std::endl;
    std::cout << "4. Показати лікарів" << std::endl;
    std::cout << "5. Закріпити лікаря за палатою" << std::endl;
    std::cout << "6. Додати пацієнта" << std::endl;
    std::cout << "7. Показати пацієнтів" << std::endl;
    std::cout << "8. Додати аналіз пацієнту" << std::endl;
    std::cout << "9. Виписати пацієнта" << std::endl;
    std::cout << "0. Вийти" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Введіть ваш вибір: ";
}

int main() {
    int choice;
    do {
        display_menu();
        while (!(std::cin >> choice)) {
            std::cout << "Некоректний ввід. Будь ласка, введіть число: ";
            std::cin.clear();
            clear_input_buffer();
        }

        switch (choice) {
            case 1: add_department(); break;
            case 2: display_departments(); break;
            case 3: add_doctor(); break;
            case 4: display_doctors(); break;
            case 5: assign_doctor_to_ward(); break;
            case 6: add_patient(); break;
            case 7: display_patients(); break;
            case 8: add_analysis_to_patient(); break;
            case 9: discharge_patient(); break;
            case 0: std::cout << "Вихід з програми. До побачення!" << std::endl; break;
            default: std::cout << "Некоректний вибір. Спробуйте ще раз." << std::endl; break;
        }
    } while (choice != 0);

    return 0;
}
