#include <stdlib.h>

#include <mupdf/fitz.h>
#include <mupdf/pdf.h>


void usage(char const *prog) {
    fprintf(stdout, "%s [infile] [outfile]", prog);
}

int handle_page(fz_context *ctx, fz_stext_page *page, FILE *fp) {
    int rc = 0;
    fz_buffer *buffer = NULL;
    fz_output *output = NULL;

    buffer = fz_new_buffer(ctx, 0xff);
    if (!buffer)
        goto failure;
    output = fz_new_output_with_buffer(ctx, buffer);
    if (!output)
        goto failure;
    fz_print_stext_page_as_text(ctx, output, page);
    fprintf(fp, "%*.*s",
            (int) buffer->len,
            (int) buffer->len,
            (char const*) buffer->data);

    goto success;
exit:
    return rc;
success:
    rc = 1;
    goto cleanup;
failure:
    rc = 0;
    goto cleanup;
cleanup:
    if (output) {
        fz_close_output(ctx, output);
        fz_drop_output(ctx, output);
        output = NULL;
    }
    if (buffer) {
        fz_drop_buffer(ctx, buffer);
        buffer = NULL;
    }
    goto exit;
}

int main(int argc, char* *argv) {
    int rc = 0;
    fz_context *ctx = NULL;
    fz_document *doc = NULL;
    fz_stext_page *page = NULL;
    char const *path = NULL;
    int npages = 0;
    FILE *fp = stdout;
    char const *filename = NULL;
    int i = 0, n = 0;

    if (argc < 2) {
        usage(0);
        return 1;
    }
    path = argv[1];
    if (argc > 2) {
        filename = argv[2];
        fp = fopen(filename, "w");
        if (!fp) {
            perror("fopen");
            goto failure;
        }
    }

    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!ctx)
        goto failure;
    fz_register_document_handlers(ctx);
    doc = fz_open_document(ctx, path);
    if (!doc)
        goto failure;
    npages = fz_last_page(ctx, doc).page + 1;
    for (i = 0, n = npages; i < n; ++i) {
        page = fz_new_stext_page_from_page_number(ctx, doc, i, NULL);
        if (!page)
            continue;
        if (!handle_page(ctx, page, fp))
            goto failure;
        fz_drop_stext_page(ctx, page);
        page = NULL;
    }

    goto success;
exit:
    return rc;
success:
    rc = EXIT_SUCCESS;
    goto cleanup;
failure:
    rc = EXIT_FAILURE;
    goto cleanup;
cleanup:
    if (ctx) {
        if (page) {
            fz_drop_stext_page(ctx, page);
            page = NULL;
        }
        if (doc) {
            fz_drop_document(ctx, doc);
            doc = NULL;
        }
        fz_drop_context(ctx);
        ctx = NULL;
    }
    if (filename && fp != stdout) {
        fclose(fp);
        fp = NULL;
    }
    goto exit;
}
