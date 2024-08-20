#include "../../include/db.h"
#include "../../include/utilities.h"


// TODO : A Regular expression pattern for validating usernames. The pattern ensures that the
//? usernames can contain lowercase, digits, and underscores, but can only start with letters
static const char *unamePattern   = "^[a-z][a-z0-9_]*$";

// TODO : Regular expression patterns for validating passwords. The pattern ensures that the
//? passwords must have atleast 1 lowercase, 1 digit, and uppercase, and 1 special character
static const char *digitPattern   = ".*[0-9].*";
static const char *upperPattern   = ".*[A-Z].*";
static const char *lowerPattern   = ".*[a-z].*";
static const char *specialPattern = ".*[^a-zA-Z0-9].*";


// TODO : A Regular expression structures for username and password validation.
static regex_t __uname, __passwd;


/**
 * @brief This function matches a string against a regex pattern.
 *
 * @param regex A pointer to the regex structure.
 * @param pattern A pointer to the regex pattern.
 * @param string A poiinter to the string to match against the pattern.
 *
 * @returns true if the string matches the pattern, false otherwise.
 */
bool regex_match(regex_t *regex, const char *pattern, const char *string) {
    if (NULL == regex || pattern == NULL || string == NULL) {
        log_error(__func__, __FILE__, __LINE__, "NULL input parameters!...\n");
        return false;
    }

    int ret = regcomp(regex, pattern, REG_EXTENDED);
    if (ret != REG_NOERROR) {
        char errbuf[MIN_BUFF];
        regerror(ret, regex, errbuf, sizeof(errbuf));
        log_error(__func__, __FILE__, __LINE__, "Failed to compile regex : errBuf => %s!...\n", errbuf);
        return false;
    }

    ret = regexec(regex, string, 0, NULL, 0);
    if (ret == REG_NOERROR) {
        regfree(regex);
        return true;
    }

    if (ret == REG_NOMATCH) {
        regfree(regex);
        log_error(__func__, __FILE__, __LINE__, "regexec : REG_NOMATCH!...\n");
        return false;
    }

    char msgbuf[MIN_BUFF];
    regerror(ret, regex, msgbuf, sizeof(msgbuf));
    log_error(__func__, __FILE__, __LINE__, "Regex match failed : errBuf => %s!...\n", msgbuf);
    regfree(regex);
    return false;
}


/**
 * @brief This function checks if a given password is valid.
 *
 * @param passwd A pointer to the password string to validate.
 *
 * @returns true if the password is valid, false otherwise.
 */
bool is_valid_passwd(const char *passwd) {
    regex_t digitRegex, upperRegex, lowerRegex, specialRegex;
    if (!regex_match(&digitRegex, digitPattern, passwd)) {
        regfree(&digitRegex);
        return false;
    }
    regfree(&digitRegex);

    if (!regex_match(&upperRegex, upperPattern, passwd)) {
        regfree(&upperRegex);
        return false;
    }
    regfree(&upperRegex);

    if (!regex_match(&lowerRegex, lowerPattern, passwd)) {
        regfree(&lowerRegex);
        return false;
    }
    regfree(&lowerRegex);

    if (!regex_match(&specialRegex, specialPattern, passwd)) {
        regfree(&specialRegex);
        return false;
    }
    regfree(&specialRegex);

    regfree(&__passwd);
    const size_t pLen = strlen(passwd);
    return pLen >= MIN_PASSWD_LEN && pLen <= MAX_PASSWD_LEN;
}


/**
 * @brief This function checks if a given username is valid.
 *
 * @param uname A pointer to the username string to validate.
 *
 * @returns true if the username is valid, false otherwise.
 */
bool is_valid_uname(const char *uname) {
    const bool regMatch = regex_match(&__uname, unamePattern, uname);
    regfree(&__uname);
    const size_t uLen = strlen(uname);
    return regMatch && uLen >= MIN_UNAME_LEN && uLen <= MAX_UNAME_LEN;
}


/**
 * @brief This function calls both functions to validate a username and password.
 *
 * @param uname A pointer to the username string to validate.
 * @param passwd A pointer to the password string to validate.
 *
 * @returns true if both username and password meet the minimum requirements, false otherwise.
 */
bool valid_credentials(const char *uname, const char *passwd) {
    const bool validUname = is_valid_uname(uname);
    const bool validPasswd = is_valid_passwd(passwd);

    return validUname && validPasswd;
}


/**
 * @brief This function hashes a user password using SHA256 algorithm.
 *
 *        The function was written with the help of the doucmentation and code samples
 *        on the openssl.org website. See the reference link below.
 *
 * @param passwd A pointer to the password string to be hashed.
 *
 * @returns A dynamically allocated string containing the hashed password.
 *         The caller is responsible for freeing the allocated memory.
 *
 * @see https://www.openssl.org/docs/manmaster/man3/EVP_MD_CTX_new.html
 */
char *sha256_hashpass(const char *passwd) {
    //TODO : Allocate memory for the hash value (32 bytes + null terminator)
    char *hashedString = calloc(SHA256_DIGEST_LENGTH * 2 + 1, sizeof(char));
    if (hashedString == NULL) {
        log_error(__func__, __FILE__, __LINE__, "calloc() %s \n", strerror(ENOMEM));
        return NULL;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (ctx == NULL) {
        free(hashedString);
        log_error(__func__, __FILE__, __LINE__, "EVP_MD_CTX_new()\n");
        return NULL;
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != EVP_SUCCESS) {
        EVP_MD_CTX_free(ctx);
        free(hashedString);
        log_error(__func__, __FILE__, __LINE__, "EVP_DigestInit_ex()\n");
        return NULL;
    }

    if (EVP_DigestUpdate(ctx, passwd, strlen(passwd)) != EVP_SUCCESS) {
        EVP_MD_CTX_free(ctx);
        free(hashedString);
        log_error(__func__, __FILE__, __LINE__, "EVP_DigestUpdate()\n");
        return NULL;
    }

    unsigned char digest[SHA256_DIGEST_LENGTH];
    if (EVP_DigestFinal_ex(ctx, digest, NULL) != EVP_SUCCESS) {
        EVP_MD_CTX_free(ctx);
        free(hashedString);
        log_error(__func__, __FILE__, __LINE__, "EVP_DigestFinal_ex()\n");
        return NULL;
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(&hashedString[i * 2], "%02x", digest[i]);
    }

    hashedString[SHA256_DIGEST_LENGTH * 2] = NULL_TERM;
    EVP_MD_CTX_free(ctx);
    return hashedString;
}


/**
 * @brief This function checks the availability of a given username by checking
 *        if there is an existing user account with the specified username in the database.
 *
 * @param uname The username to be checked for duplication.
 *
 * @returns true if a duplicate user account is found, false otherwise.
 */
bool is_username_taken(const char *uname) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_open_v2() : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    const char *QUERY = "SELECT username FROM accounts WHERE username = ?";
    if (sqlite3_prepare_v3(db, QUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_prepare_v3() : %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, uname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_bind_text() : %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    const int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    if (rc != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_step() : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    return false;
}


/**
 * @brief This function validates login credentials by checking if the provided username and password
 *        match an existing user account in the database.
 *
 * @param Uname A pointer to the username to be verified.
 * @param Passwd A pointer to the password to be verified.
 *
 * @returns true if the login details are valid, false otherwise.
 */
bool is_login_valid(const char *Uname, const char *Passwd) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_open_v2() : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    const char *QUERY = "SELECT player_id FROM accounts WHERE username = ? AND password = ?";
    if (sqlite3_prepare_v3(db, QUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_prepare_v3() : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    char *hashedPassword = sha256_hashpass(Passwd);
    if (hashedPassword == NULL) {
        log_error(__func__, __FILE__, __LINE__, "[hPass] : %s\n", strerror(ENOMEM));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit_program(EXIT_FAILURE);
    }

    if (sqlite3_bind_text(stmt, 1, Uname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_bind_text() : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(hashedPassword);
        return false;
    }

    if (sqlite3_bind_text(stmt, 2, hashedPassword, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_bind_text() : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(hashedPassword);
        return false;
    }

    const int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(hashedPassword);
        return true;
    }
    if (rc != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "sqlite3_step() : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    free(hashedPassword);

    return false;
}