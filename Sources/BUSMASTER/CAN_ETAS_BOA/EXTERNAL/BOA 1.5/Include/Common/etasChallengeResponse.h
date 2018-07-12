/**

@file

This file declares helper functions to assist in two use cases:

- A DLL developer wishes to write a DLL which can participate in ETAS's DLL-authentication mechanism. To do this,
  his DLL must implement and export a function called ETAS_ChallengeResponse(), which is documented below. The helper
  function EtasSignChallenge() assists him in doing this.

- An ETAS developer wishes to authenticate a DLL which he has loaded. To do this, he must use function ETAS_ChallengeResponse(),
  which is exported by the DLL. The helper function EtasDoChallengeResponse() calls ETAS_ChallengeResponse() on his behalf, and
  processes the results for him.

ETAS's DLL authentication mechanism is described here: @ref DllAuthentication. Note that the mechanism only applies to
Windows DLLs. QNX DLLs are never authenticated.

@copyright  Copyright (c) 2011 ETAS GmbH. All rights reserved.

$Revision: 5041 $

*/

/* DLL authentication is only supported on Windows. */
#if defined( _WIN32 ) || defined( DOXYGEN_RUNNING )

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @page DllAuthentication ETAS's DLL authentication mechanism
     *
     * ETAS's mechanism for authenticating a DLL relies upon "challenge response": the module which loads the DLL issues a value
     * called the "challenge", and the DLL is expected to authenticate itself via an appropriate response.
     *
     * The authentication mechanism is used by the BOA framework to authenticate OCD DLLs, and the results of this authentication
     * are exposed by the API CSI_GetOcdAuthInfo(). A BOA client may refuse to use a device whose OCD was not authenticated
     * successfully, but a BOA client is also free to ignore the authentication status of the OCD DLLs.
     *
     * The authentication mechanism can also be used by a BOA client to authenticate the BOA framework DLLs themselves. This is
     * necessary if the client wishes to have confidence in the authentication of OCD DLLs which is performed by the framework
     * (i.e. it is necessary to ensure that an attacker has not substituted an evil BOA framework which "authenticates" the
     * attacker's evil OCD DLL).
     *
     * In order to to use the authentication mechanism to authenticate the BOA framework DLLs, a BOA client must apply the helper
     * function EtasDoChallengeResponse() to any BOA framework DLL which he loads.
     *
     * The operation of the mechanism is best described in two stages: preparation and runtime.
     *
     * Preparation stage:
     *  -# ETAS uses the ETAS tool GenerateKeyPair.exe to generate a master public/private key pair which ETAS will use to authenticate
     *     DLL developers. To improve security, it is recommended that different master key pairs are generated for each DLL domain.
     *     For example, BOA OCD DLLs should use one master key pair, while INCA A1B DLLs should use a separate master key pair.
     *  -# A DLL developer uses the ETAS tool GenerateKeyPair.exe to generate his own public/private key pair.
     *  -# The DLL developer sends his public key, in a binary file, to ETAS.
     *  -# ETAS signs the DLL developer's public key, using ETAS's master private key obtained in step 1 and the KeySigner tool, and
     *     returns the signature, in a text file, to the DLL developer.
     *  -# The DLL developer embeds his public/private key pair (obtained in step 2) and ETAS's signature (obtained in step 4)
     *     into his DLL.
     *  -# The DLL developer implements a function called ETAS_ChallengeResponse() in his DLL. A helper function exists to assist
     *     him in doing this: see the documentation for @ref ETAS_ChallengeResponse() and EtasSignChallenge() below.
     *
     * Runtime stage:
     *  -# ETAS's software application calls the function ETAS_ChallengeResponse() which is exported by the DLL, passing a
     *     block of random data called a "challenge".
     *  -# ETAS_ChallengeResponse() outputs the following information, with the assistance of the helper function EtasSignChallenge():
     *      - A signature calculated over the challenge value and the DLL's own handle. The signature must be calculated using
     *        the SHA-1 (hash) and RCA (encryption) algorithms, using the  private key which the DLL developer chose for himself
     *        in the preparation stage.
     *      - The DLL developer's public key, corresponding to the private key used in the previous step.
     *      - ETAS's signature of the DLL developer's public key, as obtained during the preparation stage.
     *  -# The ETAS software application must use the information returned by ETAS_ChallengeResponse() to perform the following
     *     checks:
     *      - Use ETAS's master public key to verify the signature of the DLL developer's public key.
     *      - Check that the DLL developer's public key has not been revoked (this requires to existence of a central repository
     *        of revoked keys).
     *      - Use the DLL developer's public key to verify the signature of the challenge value.
     *
     * A helper function EtasDoChallengeResponse() exists to assist the ETAS software application in performing the runtime stage.
     *
     * Note that the DLL authentication mechanism only applies to Windows DLLs. QNX DLLs are never authenticated.
     */

#define AUTH_SUCCESS                0
#define AUTH_ERR_CRYPTO_PROVIDER    1
#define AUTH_ERR_INVALID_DLL_KEY    2
#define AUTH_ERR_INVALID_ETAS_KEY   3
#define AUTH_ERR_CREATE_HASH        4
#define AUTH_ERR_SIGN_HASH          5
#define AUTH_ERR_NOT_SUPPORTED      6
#define AUTH_ERR_GEN_CHALLENGE      7
#define AUTH_ERR_VERIFY_DLL_KEY     8
#define AUTH_ERR_KEY_REVOKED        9
#define AUTH_ERR_VERIFY_CHALLENGE   10
#define AUTH_ERR_DEPENDENT_DLL      11
#define AUTH_ERR_INVALID_ARG        12

#define PUB_KEY_SIZE    276    /**< We assume that public keys are always this size (we always use the RSA algorithm). */
#define KEY_PAIR_SIZE   1172   /**< We assume that public/private key pairs are always this size (we always use the RSA algorithm). */
#define SIG_SIZE        256    /**< We assume that signatures are always this size (we always use the RSA algorithm). */

#if defined( DOXYGEN_RUNNING )
    /**
     * This function must be exported by any DLL which wishes to be authenticated by an ETAS software application.
     * The authentication mechanism is described here: @ref DllAuthentication.
     *
     * The recommended approach to implement ETAS_ChallengeResponse() is as follows:
     *  -# The DLL developer uses the ETAS tool GenerateKeyPair.exe to generate a public/private key pair.
     *  -# The DLL developer sends the public key, in a binary file, to ETAS.
     *  -# ETAS signs the public key, using ETAS's master private key and the KeySigner tool, and returns the signature,
     *     in a text file, to the DLL developer.
     *  -# The DLL developer embeds his public/private key pair (obtained in step 1) and ETAS's signature (obtained in step 3)
     *     into his DLL.
     *  -# The DLL developer implements ETAS_ChallengeResponse() in his DLL as follows:
    @verbatim
        BYTE myPubPrivKeyPair[] = {...};
        BYTE myPubKey[] = {...};
        BYTE etasSigOfMyPubKey[] = {...};

        __declspec(dllexport) int ETAS_ChallengeResponse(
            const unsigned char* pChallenge,
            unsigned long        lenChallenge,
            unsigned char*       pChallengeSig
            unsigned char*       pDllPubKey,
            unsigned char*       pDllPubKeySig )
        {
            memcpy( pDllPubKey,    myPubKey,          PUB_KEY_SIZE );
            memcpy( pDllPubKeySig, etasSigOfMyPubKey, SIG_SIZE );
            return EtasSignChallenge( myPubPrivKeyPair, pChallenge, lenChallenge, pChallengeSig );
        }
    @endverbatim
     *
     * Note that in addition to authenticating the current DLL, ETAS_ChallengeResponse() may also authenticate other DLLs
     * upon which the current DLL depends. It can do this by calling the helper function EtasDoChallengeResponse() to
     * perform the "challenge response" mechanism on each dependent DLL.
     *
     * @param[in] pChallenge        A challenge value consisting of random data.
     * @param[in] lenChallenge      The number of bytes at @p pChallenge.
     * @param[out] pChallengeSig    The caller provides a buffer containing at least @ref SIG_SIZE bytes of space. The function must
     *                              fill it with a signature obtained as follows: calculate an SHA-1 hash over @p pChallenge
     *                              plus the DLL's handle; encrypt the hash using the RSA algorithm and the private key
     *                              corresponding to @p pDllPubKey. In practice, the DLL should use the function
     *                              EtasSignChallenge() within this library to calculate the value of @p pChallengeSig.
     * @param[out] pDllPublicKey    The caller provides a buffer containing at least @ref PUB_KEY_SIZE bytes of space. The function
     *                              must fill it with the public key corresponding to the private key used during the creation
     *                              of @p pChallengeSig.
     * @param[out] pDllPublicKeySig The caller provides a buffer containing at least @ref SIG_SIZE bytes of space. The function must
     *                              fill it with a signature over @p pDllPublicKey, given to the DLL developer by ETAS. This
     *                              value is calculated by ETAS using the KeySigner tool.
     *
     * @return This function must return one of the following values:
     *  - AUTH_ERR_INVALID_ARG if @p pChallenge is NULL, @p lenChallenge is zero, or any of the output arguments are NULL.
     *  - AUTH_ERR_DEPENDENT_DLL if ETAS_ChallengeResponse() failed to authenticate a DLL upon which the current DLL depends.
     *  - Any return value documented for EtasSignChallenge().
     */

    /* We only include this declaration when Doxygen is running, because we do not actually implement this function ourselves. */
    int ETAS_ChallengeResponse( const unsigned char* pChallenge, unsigned long lenChallenge, unsigned char* pChallengeSig, unsigned char* pDllPubKey, unsigned char* pDllPubKeySig );

#endif

    /**
     * This function calculates the @p pChallengeSig value required by an implementor of ETAS_ChallengeResponse(). This function
     * is intended to be linked into the DLL which is required to be authenticated. The authentication mechanism is described
     * here: @ref DllAuthentication.
     *
     * @param[in]  pDllKeyPair  The public/private key pair which belongs to the owner of the DLL into which this function
     *                          has been linked. This is assumed to be @ref KEY_PAIR_SIZE bytes long
     * @param[in]  pChallenge   The challenge value received from the module which is attempting to load the DLL into which
     *                          this function has been linked.
     * @param[in]  lenChallenge The number of bytes at @pChallenge.
     * @param[out] pSig         A buffer containing at least @ref SIG_SIZE bytes of space, which will be filled with the signaure
     *                          of @p pChallenge.
     *
     * @return
     * - AUTH_SUCCESS               Success.
     * - AUTH_ERR_CRYPTO_PROVIDER   Failed to access the RSA_FULL crypto provider.
     * - AUTH_ERR_INVALID_DLL_KEY   @p pDllKeyPair is not a valid key pair. @p pDllKeyPair should have been created by the ETAS
     *                              tool GenerateKeyPair.exe
     * - AUTH_ERR_CREATE_HASH       Failed to create a hash of the challenge value.
     * - AUTH_ERR_SIGN_HASH         Failed to sign the hash of the challenge value.
     */
    int EtasSignChallenge(
        const unsigned char* pDllKeyPair,
        const unsigned char* pChallenge,
        unsigned long        lenChallenge,
        unsigned char*       pSig
    );

    /**
     * This function indicates whether a given public key has been revoked. Typically it is implemented by a central authority
     * which maintains a list of revoked public keys.
     *
     * @param[in] pKey      A public key, which is expected to contain @ref PUB_KEY_SIZE bytes.
     *
     * @return An indication of the whether @p pKey has been revoked.
     */
    typedef int (*EtasIsKeyRevokedPtr)( const unsigned char* pKey );

    /**
     * This function authenticates a given DLL. The authentication mechanism is described here: @ref DllAuthentication.
     *
     * @param[in] hTargetDll    A handle to the DLL which is to be authenticated.
     * @param[in] pEtasPubKey   This must point to a buffer containing @ref PUB_KEY_SIZE bytes, defining the master public key
     *                          corresponding to the master private key which ETAS used to sign the DLL developer's private key.
     * @param[in] pIsKeyRevoked This function provides an interface to a central repository of revoked DLL keys. If no such
     *                          repository exists (i.e. a DLL developer's public key cannot be revoked), this argument
     *                          should be NULL.
     *
     * When this function is used to authenticate a BOA DLL, whether an OCD or a BOA framework DLL, @p pEtasPubKey should refer to
     * the buffer @ref g_boaMasterPubKey, and @p pIsKeyRevoked should refer to the function CSI_IsKeyRevoked().
     *
     * @return
     * - AUTH_SUCCESS               The target DLL is authentic.
     * - AUTH_ERR_NOT_SUPPORTED     @p hTargetDll does not implement ETAS's challenge-response mechanism.
     * - AUTH_ERR_CRYPTO_PROVIDER   Failed to access the RSA_FULL crypto provider.
     * - AUTH_ERR_GEN_CHALLENGE     Failed to generate the random challenge value.
     * - AUTH_ERR_INVALID_ETAS_KEY  @p pEtasPubKey is not a valid public key. @p pEtasPubKey should have been created by the ETAS
     *                              tool GenerateKeyPair.exe
     * - AUTH_ERR_CREATE_HASH       Failed while creating a hash.
     * - AUTH_ERR_VERIFY_DLL_KEY    The public key outputted by the target DLL via ETAS_ChallengeResponse() has not been signed by ETAS.
     * - AUTH_ERR_KEY_REVOKED       The public key outputted by the target DLL via ETAS_ChallengeResponse() has been revoked.
     * - AUTH_ERR_INVALID_DLL_KEY   The public key outputted by the target DLL via ETAS_ChallengeResponse() is not a well-formed public key.
     * - AUTH_ERR_VERIFY_CHALLENGE  The target DLL failed to correctly sign the challenge value during ETAS_ChallengeResponse().
     * - Otherwise                  Relayed from ETAS_ChallengeResponse() in the target DLL.
     */
    int EtasDoChallengeResponse(
        HMODULE             hTargetDll,
        const BYTE*         pEtasPubKey,
        EtasIsKeyRevokedPtr pIsKeyRevoked
    );

#ifdef __cplusplus
}
#endif

#endif
