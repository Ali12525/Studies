using ShopApp.Models;

namespace ShopApp.Services
{
    public interface IProductService
    {
        /// <summary>
        /// Adds a new product to the database.
        /// </summary>
        /// <param name="product">The product to add.</param>
        /// <returns>The created product entity.</returns>
        Product? Add(Product product);

        /// <summary>
        /// Removes a product from the database.
        /// </summary>
        /// <param name="id">The product identifier.</param>
        /// <returns>The removed product.</returns>
        Product? Remove(Guid id);

        /// <summary>
        /// Updates an existing product in the database.
        /// </summary>
        /// <param name="product">The product with updated data.</param>
        /// <returns>The updated product entity.</returns>
        Product? Edit(Product product);

        /// <summary>
        /// Get Information about product by id
        /// </summary>
        /// <param name="id">The ID of the product</param>
        /// <returns>Returns product if the product exists. Otherwise, it returns null</returns>
        Product? Search(Guid id);
    }
}