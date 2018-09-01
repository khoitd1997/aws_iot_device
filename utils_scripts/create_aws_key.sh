#!/usr/bin/env bash
# used for creating key the first time

set -x
set -e
aws --region us-east-1 iot create-keys-and-certificate \
    --set-as-active \
    --certificate-pem-outfile=../fs/certificate.pem.crt \
    --public-key-outfile=../fs/public.pem.key \
    --private-key-outfile=../fs/private.pem.key