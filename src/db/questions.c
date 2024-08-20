#include "../../include/db.h"

// TODO : Structure Array of science quiz questions and answers
static const quiz_t science[] = {
    {
        .question = "Which of the following is the smallest unit of matter?",
        .choices = {
            "[A]. Atom",
            "[B]. Molecule",
            "[C]. Neutron",
            "[D]. Proton"
        },
        .correct_choice = "[A]. Atom"
    },
    {
        .question = "What is the chemical symbol for the element gold?",
        .choices = {
            "[A]. Ag",
            "[B]. Fe",
            "[C]. Au",
            "[D]. Hg"
        },
        .correct_choice = "[C]. Au"
    },
    {
        .question = "Which gas is most abundant in the Earth's atmosphere?",
        .choices = {
            "[A]. Oxygen",
            "[B]. Carbon dioxide",
            "[C]. Nitrogen",
            "[D]. Hydrogen"
        },
        .correct_choice = "[C]. Nitrogen"
    },
    {
        .question = "What's the process by which plants synthesize their food?",
        .choices = {
            "[A]. Respiration",
            "[B]. Photosynthesis",
            "[C]. Osmosis",
            "[D]. Reverse Osmosis"
        },
        .correct_choice = "[B]. Photosynthesis"
    },
    {
        .question = "Which organelle produces energy in a cell?",
        .choices = {
            "[A]. Nucleus",
            "[B]. Endoplasmic reticulum",
            "[C]. Golgi apparatus",
            "[D]. Mitochondria"
        },
        .correct_choice = "[D]. Mitochondria"
    },
    {
        .question = "What is the chemical formula for water?",
        .choices = {
            "[A]. H2O",
            "[B]. CO2",
            "[C]. O2",
            "[D]. NaCl"
        },
        .correct_choice = "[A]. H2O"
    },
    {
        .question = "Which planet is known as the 'Red Planet'?",
        .choices = {
            "[A]. Venus",
            "[B]. Earth",
            "[C]. Mars",
            "[D]. Jupiter"
        },
        .correct_choice = "[C]. Mars"
    },
    {
        .question = "What is the smallest bone in the human body?",
        .choices = {
            "[A]. Femur",
            "[B]. Tibia",
            "[C]. Stapes",
            "[D]. Radius"
        },
        .correct_choice = "[C]. Stapes"
    },
    {
        .question = "Which force keeps planets in orbit around the Sun?",
        .choices = {
            "[A]. Gravitational force",
            "[B]. Magnetic force",
            "[C]. Electric force",
            "[D]. Centrifugal force"
        },
        .correct_choice = "[A]. Gravitational force"
    },
    {
        .question = "What is the chemical symbol for helium?",
        .choices = {
            "[A]. He",
            "[B]. H",
            "[C]. Ne",
            "[D]. Li"
        },
        .correct_choice = "[A]. He"
    },
    {
        .question = "The hotter the _____ the faster the rate of evaporation.",
        .choices = {
            "[A]. Water",
            "[B]. Air",
            "[C]. Temperature",
            "[D]. Soil"
        },
        .correct_choice = "[C]. Temperature"
    },
    {
        .question = "How many significant digits are there in 0.0076003?",
        .choices = {
            "[A]. 5 significant figures",
            "[B]. 8 significant figures",
            "[C]. 3 significant figures",
            "[D]. 7 significant figures"
        },
        .correct_choice = "[A]. 5 significant figures"
    },
    {
        .question = "What is the hardest naturally occurring substance on Earth?",
        .choices = {
            "[A]. Quartz",
            "[B]. Diamond",
            "[C]. Topaz",
            "[D]. Corundum"
        },
        .correct_choice = "[B]. Diamond"
    },
    {
        .question = "Which layer of atmosphere contains the ozone layer",
        .choices = {
            "[A]. Stratosphere",
            "[B]. Mesosphere",
            "[C]. Thermosphere",
            "[D]. Exosphere"
        },
        .correct_choice = "[A]. Stratosphere"
    },
    {
        .question = "Which of the following is a fossil fuel?",
        .choices = {
            "[A]. Coal",
            "[B]. Natural gas",
            "[C]. Oil",
            "[D]. All of the above"
        },
        .correct_choice = "[D]. All of the above"
    },
    {
        .question = "The appendix is attached to which part of human body?",
        .choices = {
            "[A]. Stomach",
            "[B]. Small intestine",
            "[C]. Gall bladder",
            "[D]. Large intestine"
        },
        .correct_choice = "[D]. Large intestine"
    },
    {
        .question = "Which of the following is a type of renewable energy?",
        .choices = {
            "[A]. Solar energy",
            "[B]. Wind energy",
            "[C]. Hydropower",
            "[D]. All of the above"
        },
        .correct_choice = "[D]. All of the above"
    },
    {
        .question = "What is the name of the process by which cells divide?",
        .choices = {
            "[A]. Mitosis",
            "[B]. Meiosis",
            "[C]. Both A and B",
            "[D]. None of the above"
        },
        .correct_choice = "[A]. Mitosis"
    },
    {
        .question = "Which part of the brain controls respiration?",
        .choices = {
            "[A]. Medulla oblongata",
            "[B]. Cerebellum",
            "[C]. Cerebral cortex",
            "[D]. Cerebrum"
        },
        .correct_choice = "[A]. Medulla oblongata"
    },
    {
        .question = "A surge of which hormone stimulates ovulation in females?",
        .choices = {
            "[A]. Luteinizing hormone",
            "[B]. Estrogen",
            "[C]. Follicle stimulating hormone",
            "[D]. Progesterone"
        },
        .correct_choice = "[A]. Luteinizing hormone"
    }
};


// TODO : Structure Array of sports quiz questions and answers
static const quiz_t sports[] = {
    {
        .question = "Who won the FIFA World Cup in 2018?",
        .choices = {
            "[A]. Brazil",
            "[B]. Germany",
            "[C]. France",
            "[D]. Argentina"
        },
        .correct_choice = "[C]. France"
    },
    {
        .question = "Which basketball player is often called 'The King'?",
        .choices = {
            "[A]. Kobe Bryant",
            "[B]. Michael Jordan",
            "[C]. LeBron James",
            "[D]. Shaquille O'Neal"
        },
        .correct_choice = "[C]. LeBron James"
    },
    {
        .question = "Which martial art emphasizes throws & grappling techniques?",
        .choices = {
            "[A]. Karate",
            "[B]. Taekwondo",
            "[C]. Judo",
            "[D]. Muay Thai"
        },
        .correct_choice = "[C]. Judo"
    },
    {//"Which country dominates men's tennis with Nadal and Verdasco?"
        .question = "What is the technical term for a 40-40 score in tennis?",
        .choices = {
            "[A]. Rally",
            "[B]. Deuce",
            "[C]. Overhead",
            "[D]. Deuce court"
        },
        .correct_choice = "[B]. Deuce"
    },
    {
        .question = "Which football/football team is known as the 'Red Devils'?",
        .choices = {
            "[A]. Real Madrid",
            "[B]. Manchester United",
            "[C]. Bayern Munich",
            "[D]. Liverpool"
        },
        .correct_choice = "[B]. Manchester United"
    },
    {
        .question = "Which NBA team was famous for 'Showtime' in the 1980s?",
        .choices = {
            "[A]. Boston Celtics",
            "[B]. Los Angeles Lakers",
            "[C]. Chicago Bulls",
            "[D]. Miami Heat"
        },
        .correct_choice = "[B]. Los Angeles Lakers"
    },
    {
        .question = "What martial art is known as the 'Art of Eight Limbs'?",
        .choices = {
            "[A]. Karate",
            "[B]. Jiu-Jitsu",
            "[C]. Muay Thai",
            "[D]. Taekwondo"
        },
        .correct_choice = "[C]. Muay Thai"
    },
    {
        .question = "Which football player is often referred to as 'CR7'?",
        .choices = {
            "[A]. Cristiano Ronaldo",
            "[B]. Lionel Messi",
            "[C]. Neymar",
            "[D]. Gareth Bale"
        },
        .correct_choice = "[A]. Cristiano Ronaldo"
    },
    {
        .question = "How many points is a three-point field goal in basketball?",
        .choices = {
            "[A]. 1",
            "[B]. 2",
            "[C]. 3",
            "[D]. 4"
        },
        .correct_choice = "[C]. 3"
    },
    {
        .question = "Which of the following martial art originated in South Korea?",
        .choices = {
            "[A]. Karate",
            "[B]. Judo",
            "[C]. Taekwondo",
            "[D]. Kung Fu"
        },
        .correct_choice = "[C]. Taekwondo"
    },
    {
        .question = "Which team has won the most FIFA World Cups?",
        .choices = {
            "[A]. Brazil",
            "[B]. Germany",
            "[C]. Italy",
            "[D]. Argentina"
        },
        .correct_choice = "[A]. Brazil"
    },
    {
        .question = "Which of the following NBA players has the highest points?",
        .choices = {
            "[A]. LeBron James",
            "[B]. Kareem Abdul-Jabbar",
            "[C]. Michael Jordan",
            "[D]. Wilt Chamberlain"
        },
        .correct_choice = "[A]. LeBron James"
    },
    {
        .question = "Who holds the record for the fastest 100m sprint in history?",
        .choices = {
            "[A]. Justin Gatlin",
            "[B]. Asafa Powell",
            "[C]. Usain Bolt",
            "[D]. Carl Lewis"
        },
        .correct_choice = "[C]. Usain Bolt"
    },
    {
        .question = "Which player scored the fastest hat-trick in Premier League?",
        .choices = {
            "[A]. Jebron Lames",
            "[B]. Gareth Bale",
            "[C]. Sadio Mane",
            "[D]. Cristiano Ronaldo"
        },
        .correct_choice = "[C]. Sadio Mane"
    },
    {
        .question = "Which NBA team has made the most appearances in the finals?",
        .choices = {
            "[A]. Los Angeles Lakers",
            "[B]. Toronto Raptors",
            "[C]. Boston Celtics",
            "[D]. Chicago Bulls"
        },
        .correct_choice = "[A]. Los Angeles Lakers"
    },
    {
        .question = "What martial art is a traditional sword martial art?",
        .choices = {
            "[A] Karate",
            "[B] Taekwondo",
            "[C] Muay Thai",
            "[D] Headong kumdo"
        },
        .correct_choice = "[D] Headong kumdo"
    },
    {
        .question = "As of 2023, which player has won the most Ballon d'Or awards?",
        .choices = {
            "[A] Lionel Messi",
            "[B] Cristiano Ronaldo",
            "[C] Johan Cruyff",
            "[D] Michel Platini"
        },
        .correct_choice = "[A] Lionel Messi"
    },
    {
        .question = "Which basketball team has won the most NBA championships?",
        .choices = {
            "[A] Boston Celtics",
            "[B] Los Angeles Lakers",
            "[C] Golden State Warriors",
            "[D] Chicago Bulls"
        },
        .correct_choice = "[B] Boston Celtics"
    },
    {
        .question = "Who is the founder of the 'Wadō-ryū' martial arts?",
        .choices = {
            "[A] Keigo Abe",
            "[B] Satoshi Nakamoto",
            "[C] Hironori Ōtsuka",
            "[D] William Kwai-sun Chow"
        },
        .correct_choice = "[C] Hironori Ōtsuka"
    },
    {
        .question = "Which of the following tournaments is held every four years?",
        .choices = {
            "[A] FIFA World Cup",
            "[B] UEFA European Championship",
            "[C] Copa América",
            "[D] UEFA Champions League"
        },
        .correct_choice = "[A] FIFA World Cup"
    }
};


// TODO : Structure Array of basic/general knowledge quiz questions and answers
static const quiz_t general_knowledge[] = {
    {
        .question = "What is the largest planet in our solar system?",
        .choices = {
            "[A]. Venus",
            "[B]. Earth",
            "[C]. Jupiter",
            "[D]. Mars"
        },
        .correct_choice = "[C]. Jupiter"
    },
    {
        .question = "Which river is the longest in the world?",
        .choices = {
            "[A]. Amazon River",
            "[B]. Nile River",
            "[C]. Mississippi River",
            "[D]. Yangtze River"
        },
        .correct_choice = "[B]. Nile River"
    },
    {
        .question = "Who is the author of 'To Kill a Mockingbird'?",
        .choices = {
            "[A]. J.K. Rowling",
            "[B]. Harper Lee",
            "[C]. George Orwell",
            "[D]. Charles Dickens"
        },
        .correct_choice = "[B]. Harper Lee"
    },
    {
        .question = "Which country is known as the Land of the Rising Sun?",
        .choices = {
            "[A]. China",
            "[B]. Japan",
            "[C]. India",
            "[D]. South Korea"
        },
        .correct_choice = "[B]. Japan"
    },
    {
        .question = "What is the largest ocean on Earth?",
        .choices = {
            "[A]. Atlantic Ocean",
            "[B]. Indian Ocean",
            "[C]. Arctic Ocean",
            "[D]. Pacific Ocean"
        },
        .correct_choice = "[D]. Pacific Ocean"
    },
    {
        .question = "Who painted the Mona Lisa?",
        .choices = {
            "[A]. Vincent van Gogh",
            "[B]. Pablo Picasso",
            "[C]. Leonardo da Vinci",
            "[D]. Michelangelo"
        },
        .correct_choice = "[C]. Leonardo da Vinci"
    },
    {
        .question = "What is the currency of Mexico?",
        .choices = {
            "[A]. Peso",
            "[B]. Euro",
            "[C]. Dollar",
            "[D]. Yen"
        },
        .correct_choice = "[A]. Peso"
    },
    {
        .question = "Which among the following is known as the City of Love?",
        .choices = {
            "[A]. Abuja",
            "[B]. Paris",
            "[C]. Barcelona",
            "[D]. Istanbul"
        },
        .correct_choice = "[B]. Paris"
    },
    {
        .question = "In which year did the Titanic sink?",
        .choices = {
            "[A]. 1907",
            "[B]. 1912",
            "[C]. 1920",
            "[D]. 1931"
        },
        .correct_choice = "[B]. 1912"
    },
    {
        .question = "Who was the first woman to fly solo across the Atlantic?",
        .choices = {
            "[A]. Amelia Earhart",
            "[B]. Bessie Coleman",
            "[C]. Charles Lindbergh",
            "[D]. Howard Hughes"
        },
        .correct_choice = "[A]. Amelia Earhart"
    },
    {
        .question = "What is the capital of France?",
        .choices = {
            "[A]. London",
            "[B]. Berlin",
            "[C]. Paris",
            "[D]. Rome"
        },
        .correct_choice = "[C]. Paris"
    },
    {
        .question = "What is the largest ocean in the world?",
        .choices = {
            "[A]. Atlantic Ocean",
            "[B]. Pacific Ocean",
            "[C]. Indian Ocean",
            "[D]. Arctic Ocean"
        },
        .correct_choice = "[B]. Pacific Ocean"
    },
    {
        .question = "What is the most populous country in the world?",
        .choices = {
            "[A]. China",
            "[B]. India",
            "[C]. United States",
            "[D]. Indonesia"
        },
        .correct_choice = "[A]. China"
    },
    {
        .question = "What is the name of the world's tallest mountain?",
        .choices = {
            "[A]. Mount Everest",
            "[B]. K2",
            "[C]. Kangchenjunga",
            "[D]. Lhotse"
        },
        .correct_choice = "[A]. Mount Everest"
    },
    {
        .question = "What is the name of the largest desert in the world?",
        .choices = {
            "[A]. Sahara Desert",
            "[B]. Arabian Desert",
            "[C]. Gobi Desert",
            "[D]. Arctic Desert"
        },
        .correct_choice = "[A]. Sahara Desert"
    },
    {
        .question = "What is the name of the most famous painting in the world?",
        .choices = {
            "[A]. Mona Lisa",
            "[B]. Starry Night",
            "[C]. The Scream",
            "[D]. Guernica"
        },
        .correct_choice = "[A]. Mona Lisa"
    },
    {
        .question = "what's the most famous book in the world as of 1995?",
        .choices = {
            "[A]. The Bible",
            "[B]. Quran",
            "[C]. The Lord of the Rings",
            "[D]. Harry Potter"
        },
        .correct_choice = "[A]. The Bible"
    },
    {
        .question = "What is the name of the longest river in the world?",
        .choices = {
            "[A]. Nile River",
            "[B]. Amazon River",
            "[C]. Yangtze River",
            "[D]. Mississippi River"
        },
        .correct_choice = "[A]. Nile River"
    },
    {
        .question = "What's the most popular social media platform in the world?",
        .choices = {
            "[A]. Facebook",
            "[B]. Instagram",
            "[C]. Twitter",
            "[D]. TikTok"
        },
        .correct_choice = "[A]. Facebook"
    },
    {
        .question = "What is the name of the largest city in the world?",
        .choices = {
            "[A]. Tokyo",
            "[B]. Shanghai",
            "[C]. Delhi",
            "[D]. São Paulo"
        },
        .correct_choice = "[A]. Tokyo"
    }
};


/**
 * @brief This function inserts quiz data (questions, choices, and correct choice) into their respective tables.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param quizArray An array of quiz_t structures containing quiz question & answer data.
 * @param arraySize The size of the quizArray.
 * @param tableName The name of the table to insert data into.
 *
 * @returns true if the quiz questions and answers are successfully inserted to their respective tables, otherwise false.
 */
bool insert_quiz_data(sqlite3 *db, const quiz_t *quizArray, const size_t arraySize, const char *tableName) {
    if (db == NULL || quizArray == NULL || arraySize <= 1 || tableName == NULL) {
        fprintf(stderr, " [%s] Error : Invalid input parameters!...\n", WARNING_EMOJI);
        return false;
    }

    char insertQUERY[MAX_BUFF * 2];
    snprintf(insertQUERY, sizeof(insertQUERY),
             "INSERT INTO %s (question_id, question, choice_a, choice_b, choice_c, choice_d, correct_choice) "
             "VALUES (NULL, ?, ?, ?, ?, ?, ?)", tableName);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, insertQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    // Bind values and execute the statement for each column
    for (size_t i = 0; i < arraySize; i++) {
        if (sqlite3_bind_text(stmt, 1, quizArray[i].question, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 2, quizArray[i].choices[0], NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 3, quizArray[i].choices[1], NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 4, quizArray[i].choices[2], NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 5, quizArray[i].choices[3], NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 6, quizArray[i].correct_choice, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
                log_error(__func__, __FILE__, __LINE__, "Failed to bind values : %s!...\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                return false;
        }

        //? Execute & reset the statement for the next iteration
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_finalize(stmt);

    print(" [%s] Data successfully inserted to '%s' table!...\n", SUCCESS_EMOJI, tableName);
    return true;
}


/**
 * @brief This function calls the 'insert_quiz_data' function which inserts all the quiz questions and answers
 *        with the correct choice to their respective tables in the database
 *
 * @param db A pointer to the database to insert the quiz data
 *
 * @return true if data functions execute successfully, otherwise false.
 */
bool insert_quiz_questions_answers(sqlite3 *db) {
    if (db != NULL) {
        if (insert_quiz_data(db, science, sizeof(science) / sizeof(science[0]), "Science") &&
            insert_quiz_data(db, sports, sizeof(sports) / sizeof(sports[0]), "Sports") &&
            insert_quiz_data(db, general_knowledge, sizeof(general_knowledge) / sizeof(general_knowledge[0]), "Basics")) {
                print(" [%s] DATA inserted successfully!...\n", SUCCESS_EMOJI);
                return true;
            }
        log_error(__func__, __FILE__, __LINE__, "Failed to insert data to database!...\n");
        sqlite3_close(db);
        return false;
    }
    log_error(__func__, __FILE__, __LINE__, "NULL DB connection!...\n");
    return false;
}
