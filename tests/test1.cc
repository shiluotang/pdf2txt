#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <new>

#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

#include "../src/context.hpp"
#include "../src/document.hpp"
#include "../src/page.hpp"
#include "../src/stream.hpp"
#include "../src/ref_counted.hpp"

void pdf_manip() {
}

int main(int argc, char* argv[]) try {
    using namespace std;
    using namespace fitz;

    std::string const path = "/Users/shengquangang/Downloads/OMA-ERP-SUPL-V2_0_5-20191028-A/OMA-TS-ULP-V2_0_5-20191028-A.pdf";
    ::fz_context *ctx = ::fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    ::fz_register_document_handlers(ctx);
    ::fz_document *doc = ::fz_open_document(ctx, path.c_str());
    int npages = fz_last_page(ctx, doc).page + 1;
    cout << "There're " << npages << " page(s)." << endl;

    for (int i = 0, n = npages; i < n; ++i) {
        ::fz_page *page = ::fz_load_page(ctx, doc, i);
        if (page) {
        ::fz_stext_page *spage = ::fz_new_stext_page_from_page(
                ctx, page, NULL);
            if (spage) {
                ::fz_buffer *buffer = ::fz_new_buffer(ctx, 0xff);
                ::fz_output *output = ::fz_new_output_with_buffer(
                        ctx, buffer);
                ::fz_print_stext_page_as_text(ctx, output, spage);
                printf("%*.*s",
                        (int) buffer->len,
                        (int) buffer->len,
                        (char const*) buffer->data);
                ::fz_drop_output(ctx, output); output = NULL;
                ::fz_drop_buffer(ctx, buffer); buffer = NULL;
                ::fz_drop_stext_page(ctx, spage); spage = NULL;
            }
        }
        if (page) {
            ::fz_drop_page(ctx, page);
            page = NULL;
        }
    }


    return EXIT_SUCCESS;
} catch (std::exception const &e) {
    std::cerr << "[C++ exception] " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "[C++ exception] " << "<UNKNOWN CAUSE>" << std::endl;
    return EXIT_FAILURE;
}
