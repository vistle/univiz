void write_dump_impl(UniSys *us, Unstructured *unst, const char *file_name)
{
    if (file_name != NULL && strlen(file_name) > 0)
    {
        us->info("writing %s", file_name);

        FILE *fp;
        fp = fopen(file_name, "wb");
        if (!fp)
        {
            us->error("could not open %s", file_name);
        }

        for (int n = 0; n < unst->nNodes; n++)
        {
            vec3 v;
            unst->getVector3(n, v);
            fwrite(v, 3 * sizeof(float), 1, fp);
        }

        fclose(fp);
    }
}
