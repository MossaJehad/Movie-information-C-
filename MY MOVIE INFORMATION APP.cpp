#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;


void fetchMovieInfo(const string& movieName) {
    try {
        http_client client(U("https://api.themoviedb.org/3"));

        uri_builder builder(U("/search/movie"));
        builder.append_query(U("query"), utility::conversions::to_string_t(movieName));
        builder.append_query(U("language"), U("en-US"));
        builder.append_query(U("api_key"), utility::conversions::to_string_t("0038db482c3e5e443fcee66e5147c298"));

        http_request request(methods::GET);
        request.set_request_uri(builder.to_string());

        client.request(request).then([](http_response response) {
            return response.extract_json();
            }).then([](web::json::value body) {
                if (body[U("results")].size() > 0) {
                    auto movie = body[U("results")][0];
                    cout << "\nTitle: " << utility::conversions::to_utf8string(movie[U("title")].as_string()) << endl;
                    cout << "Rating: " << movie[U("vote_average")].as_double() << endl;
                    cout << "Overview: " << utility::conversions::to_utf8string(movie[U("overview")].as_string()) << endl;
                    cout << "Release Date: " << utility::conversions::to_utf8string(movie[U("release_date")].as_string()) << endl;
                }
                else {
                    cout << "Movie not found." << endl;
                }
                }).wait();
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {
    cout << "+---------------------------------+" << endl;
    cout << "+----MY MOVIE INFORMATION APP-----+" << endl;
    cout << "+---------------------------------+" << endl;

    while (true) {
        try {
            cout << "\n\nEnter movie name: " << endl;
            string movieName;
            cout << "> ";
            getline(cin, movieName);

            fetchMovieInfo(movieName);
        }
        catch (const std::exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}